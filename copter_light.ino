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
  blink(NW_pin, 500, 200);
  delay(500);
  blink(NE_pin, 500, 200);
  delay(500);
  blink(SE_pin, 500, 200);
  delay(500);
  blink(SW_pin, 500, 200);
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


void loop() {
  // put your main code here, to run repeatedly:

}
