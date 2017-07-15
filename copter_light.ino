static const byte NW_pin = 3;
static const byte NE_pin = 4;
static const byte SE_pin = 1;
static const byte SW_pin = 0;

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
  fade(NW_pin, 0, 255, 50);
  delay(500);
  fade(SW_pin, 0, 255, 50);
  delay(500);
  fade(SE_pin, 0, 255, 50);
  delay(500);
  fade(NE_pin, 0, 255, 50);
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

void fade(byte pin, byte from, byte to, unsigned long pauseMillis)
{
  for(byte val=from; val<to; val+=5)
  {
    analogWrite(pin, val);
    delay(pauseMillis);  
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
