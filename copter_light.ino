#include <avr/pgmspace.h>
#include <Fsm.h>

#include "globals.hpp"
#include "fsm.hpp"
#include "light_pattern.hpp"

#ifdef SE
#undef SE
#endif

enum {
  NW, NE, SE, SW, RC, Num_Pins
};

static unsigned long prevMillis = 0;

#ifdef __AVR_ATtiny85__
static const byte pin[Num_Pins] = { 3, 4, 1, 0, 2 };
#else
static const byte pin[Num_Pins] = { 5, 6, 10, 11,2  };
#endif

static const byte MAX_BRIGHT=255;
static const unsigned long stepTime = 30;

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

void setup()
{
  pinMode(pin[NW], OUTPUT);
  pinMode(pin[NE], OUTPUT);
  pinMode(pin[SE], OUTPUT);
  pinMode(pin[SW], OUTPUT);
  pinMode(pin[RC], INPUT);

  test_all();
  setup_timers();
  setup_fsm(pattern_count());
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

    if (current >= 0)
    {
      unsigned long lVal = pattern_get(current);
      pattern_advance();
      displayPattern(lVal);
    } else
      displayPattern(0);
  }
}

