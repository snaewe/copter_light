
#ifdef ARDUINO_attiny
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

void setup()
{
  pinMode(NW_pin, OUTPUT);
  pinMode(NE_pin, OUTPUT);
  pinMode(SE_pin, OUTPUT);
  pinMode(SW_pin, OUTPUT);

  test_all();
}

void test_all()
{
  blink_all();
  fade_all();
}

void blink_all()
{
  blink(NW_pin, 500, 200);
  delay(500);
  blink(NE_pin, 500, 200);
  delay(500);
  blink(SE_pin, 500, 200);
  delay(500);
  blink(SW_pin, 500, 200);
}

void fade_all()
{
  fadeUp(NW_pin, 0, 255, 50);
  fadeUp(SW_pin, 0, 255, 50);
  fadeUp(SE_pin, 0, 255, 50);
  fadeUp(NE_pin, 0, 255, 50);
  delay(500);
  fadeDown(NE_pin, 255, 0, 50);
  fadeDown(SE_pin, 255, 0, 50);
  fadeDown(SW_pin, 255, 0, 50);
  fadeDown(NW_pin, 255, 0, 50);
}

void blink(byte pin, unsigned long lightMillis, unsigned long pauseMillis) {
  digitalWrite(pin, 1);
  delay(lightMillis);
  digitalWrite(pin, 0);
  delay(pauseMillis);
  digitalWrite(pin, 1);
  delay(lightMillis);
  digitalWrite(pin, 0);
}

void fadeUp(byte pin, byte from, byte to, unsigned long pauseMillis)
{
  if (from>=to) return;

  const byte step = 5;
  for(byte val=from; val<to; val+=step)
  {
    analogWrite(pin, val);
    delay(pauseMillis);  
  }
}

void fadeDown(byte pin, byte from, byte to, unsigned long pauseMillis)
{
  if (to>=from) return;

  const byte step = 5;
  for(byte val=from; val>to; val-=step)
  {
    analogWrite(pin, val);
    delay(pauseMillis);  
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
