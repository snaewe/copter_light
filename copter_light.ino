#include <avr/pgmspace.h>

#ifdef __AVR_ATtiny85__
static const byte NW_pin = 3;
static const byte NE_pin = 4;
static const byte SE_pin = 1;
static const byte SW_pin = 0;
#else
static const byte NW_pin = 5;
static const byte NE_pin = 6;
static const byte SE_pin = 10;
static const byte SW_pin = 11;
#endif
static const byte RC_pin = 2;

static const byte MAX_BRIGHT=255;
static const byte MAX_STEPS=11;
static const unsigned long pattern[][MAX_STEPS] PROGMEM =
{
  { 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000},
  { 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF},
  { 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000},
  { 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF},
  { 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000},
  { 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF},
  { 0xff000000, 0xffff0000, 0x00ffff00, 0x0000ffff, 0x000000ff, 0},
  { 0xff000000, 0xffff0000, 0x00ffff00, 0x0000ffff, 0x000000ff, 0},
  { 0xff000000, 0xffff0000, 0x00ffff00, 0x0000ffff, 0x000000ff, 0},
  { 0, 0x1010, 0x1010, 0x2020, 0x2020, 0x4040, 0x4040, 0x8080, 0x8080, 0xffff, 0xffff },
  { 0, 0x1010, 0x1010, 0x2020, 0x2020, 0x4040, 0x4040, 0x8080, 0x8080, 0xffff, 0xffff },
  { 0, 0x1010, 0x1010, 0x2020, 0x2020, 0x4040, 0x4040, 0x8080, 0x8080, 0xffff, 0xffff },
  { 0, 0x10100000, 0x10100000, 0x20200000, 0x20200000, 0x40400000, 0x40400000, 0x80800000, 0x80800000, 0xffff0000, 0xffff0000 },
  { 0, 0x10100000, 0x10100000, 0x20200000, 0x20200000, 0x40400000, 0x40400000, 0x80800000, 0x80800000, 0xffff0000, 0xffff0000 },
  { 0, 0x10100000, 0x10100000, 0x20200000, 0x20200000, 0x40400000, 0x40400000, 0x80800000, 0x80800000, 0xffff0000, 0xffff0000 },
};
static const byte NUM_PATTERNS = sizeof(pattern) / sizeof(pattern[0]);

static byte current = 0, step = 0;
static unsigned long prevMillis = 0;

#ifdef __AVR_ATtiny85__
void setup_timers()
{
  // Configure counter/timer0 for fast PWM on PB0 and PB1
  TCCR0A = 3<<COM0A0 | 3<<COM0B0 | 3<<WGM00;
  TCCR0B = 0<<WGM02 | 3<<CS00; // Optional; already set
  // Configure counter/timer1 for fast PWM on PB4
  TCCR1 = 1<<CTC1 | 1<<PWM1A | 3<<COM1A0 | 7<<CS10;
  GTCCR = 1<<PWM1B | 3<<COM1B0;
  // Interrupts on OC1A match and overflow
  TIMSK = TIMSK | 1<<OCIE1A | 1<<TOIE1;
}

ISR(TIMER1_COMPA_vect) {
  if (!bitRead(TIFR,TOV1)) bitSet(PORTB, NW_pin);
}

ISR(TIMER1_OVF_vect) {
  bitClear(PORTB, NW_pin);
}
#else
void setup_timers(){}
#endif

void setup()
{
  pinMode(NW_pin, OUTPUT);
  pinMode(NE_pin, OUTPUT);
  pinMode(SE_pin, OUTPUT);
  pinMode(SW_pin, OUTPUT);

  test_all();
  setup_timers();
  setup_pwm_in(RC_pin);
#ifndef __AVR_ATtiny85__
  Serial.begin(115200);
#endif
}

void test_all()
{
  blink_all();
}

void blink_all()
{
  blink(NW_pin, 200, 100);
  blink(NE_pin, 200, 100);
  blink(SE_pin, 200, 100);
  blink(SW_pin, 200, 100);
}

void blink(byte pin, unsigned long lightMillis, unsigned long pauseMillis) {
  digitalWrite(pin, 1);
  delay(lightMillis);
  digitalWrite(pin, 0);
  delay(pauseMillis);
  digitalWrite(pin, 1);
  delay(lightMillis);
  digitalWrite(pin, 0);
  delay(pauseMillis);
}

template <typename T> T clip(T val, T maxVal) { return (val>maxVal) ? maxVal : val;}

void displayPattern(unsigned long lVal)
{
  byte const *b = reinterpret_cast<byte const *>(&lVal);
#ifdef __AVR_ATtiny85__
    OCR0A = 255-clip(b[0], MAX_BRIGHT);
    OCR0B = 255-clip(b[1], MAX_BRIGHT);
    OCR1A = 256-clip(b[2], MAX_BRIGHT);  // Yes, 256 is correct!
    OCR1B = 255-clip(b[3], MAX_BRIGHT);
#else
  analogWrite(NW_pin, clip(b[0], MAX_BRIGHT));
  analogWrite(NE_pin, clip(b[1], MAX_BRIGHT));
  analogWrite(SE_pin, clip(b[2], MAX_BRIGHT));
  analogWrite(SW_pin, clip(b[3], MAX_BRIGHT));
#endif
}

static unsigned long stepTime = 30;

void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis-prevMillis > stepTime)
  {
#ifndef __AVR_ATtiny85__
    Serial.println(read_pwm());
#endif
    prevMillis = currentMillis;
    unsigned long lVal = pgm_read_dword(&pattern[current][step]);
    displayPattern(lVal);
    ++step;
    if(step==MAX_STEPS)
    {
      step=0;
      ++current;
      if (current == NUM_PATTERNS)
        current = 0;
    }
  }
}
