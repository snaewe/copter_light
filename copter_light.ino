#include <avr/pgmspace.h>
#include <Fsm.h>

#ifdef SE
#undef SE
#endif

enum {
  NW, NE, SE, SW, RC, Num_Pins
};

enum Rc_Mode {
  Lower, Middle, Upper, Num_Modes
};

static const unsigned long stepTime = 30;
static int8_t current = -1, step = 0;
static unsigned long prevMillis = 0;

#ifdef __AVR_ATtiny85__
static const byte pin[Num_Pins] = { 3, 4, 1, 0, 2 };
#else
static const byte pin[Num_Pins] = { 5, 6, 10, 11,2  };
#endif

static const byte MAX_BRIGHT=255;
static const byte MAX_STEPS=22;
static const unsigned long pattern[][MAX_STEPS] PROGMEM =
{
  { 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000,0, 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF, 0},
  { 0xff000000, 0xffff0000, 0x00ffff00, 0x0000ffff, 0x000000ff, 0},
  { 0, 0x1010, 0x1010, 0x2020, 0x2020, 0x4040, 0x4040, 0x8080, 0x8080, 0xffff, 0xffff, 0, 0x10100000, 0x10100000, 0x20200000, 0x20200000, 0x40400000, 0x40400000, 0x80800000, 0x80800000, 0xffff0000, 0xffff0000 },
  { 0xff00ff00,0,0,0,0,0x00ff00ff,0,0,0,0 },
};
static const byte NUM_PATTERNS = sizeof(pattern) / sizeof(pattern[0]);

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
  if (!bitRead(TIFR,TOV1)) bitSet(PORTB, pin[NW]);
}

ISR(TIMER1_OVF_vect) {
  bitClear(PORTB, pin[NW]);
}
#else
void setup_timers(){}
#endif

void on_enter_light_off()
{
  current = -1;
  displayPattern(0x00000000); // all LEDs off
}

void current_to_zero() { current = 0; }
void increase_current() { if (current<NUM_PATTERNS) ++current; }
void decrease_current() { if (current!=-1) --current; }

State Light_Off(&on_enter_light_off, 0);
State High(0, 0);
State Neutral(0,0);
State Low(0,0);

Fsm fsm(&Light_Off);

void setup_fsm()
{
  fsm.add_transition(&Light_Off, &High, Upper, &current_to_zero);
  fsm.add_transition(&High, &Neutral, Middle, 0);
  fsm.add_transition(&Neutral, &High, Upper, &increase_current);
  fsm.add_transition(&Neutral, &Low,  Lower, &decrease_current);
  fsm.add_transition(&Low, &Neutral, Middle, 0);
}

void setup()
{
  pinMode(pin[NW], OUTPUT);
  pinMode(pin[NE], OUTPUT);
  pinMode(pin[SE], OUTPUT);
  pinMode(pin[SW], OUTPUT);
  pinMode(pin[RC], INPUT);

  test_all();
  setup_timers();
  setup_fsm();
  setup_pwm_in(pin[RC]);
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
  blink(pin[NW], 200, 100);
  blink(pin[NE], 200, 100);
  blink(pin[SE], 200, 100);
  blink(pin[SW], 200, 100);
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
  analogWrite(pin[NW], clip(b[0], MAX_BRIGHT));
  analogWrite(pin[NE], clip(b[1], MAX_BRIGHT));
  analogWrite(pin[SE], clip(b[2], MAX_BRIGHT));
  analogWrite(pin[SW], clip(b[3], MAX_BRIGHT));
#endif
}

void loop() {

  int rc_val = read_pwm();
  int rc_mode = get_mode(rc_val, Num_Modes);
  fsm.trigger(rc_mode);
  
  unsigned long currentMillis = millis();
  
  if (currentMillis-prevMillis > stepTime)
  {
#ifndef __AVR_ATtiny85__
    Serial.print(rc_val);
    Serial.print(" ");
    Serial.print(rc_mode);
    Serial.print(" ");
    Serial.println(current);
#endif
    prevMillis = currentMillis;

    if (current >= 0) {
    unsigned long lVal = pgm_read_dword(&pattern[current][step]);
    displayPattern(lVal);
    ++step;
    if(step==MAX_STEPS)
    {
      step=0;
      #if 0
      ++current;
      if (current == NUM_PATTERNS)
        current = 0;
      #endif
    }
    } else
      displayPattern(0);
  }
}


