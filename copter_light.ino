#ifdef __AVR_ATtiny85__
static const byte NW_pin = 3;
static const byte NE_pin = 4;
static const byte SE_pin = 1;
static const byte SW_pin = 0;
static const byte RC_pin = 2;
#else
static const byte NW_pin = 5;
static const byte NE_pin = 6;
static const byte SE_pin = 10;
static const byte SW_pin = 11;
static const byte RC_pin = 7;
#endif

#define MINPULSE 993
#define MAXPULSE 1987
#define MODES 3        //Anzahl der Modi
#define FLUCTUATION 8  //Pulsstreuung bzw. Pulsschwankung

unsigned int maxPulse=MINPULSE, minPulse=MAXPULSE;

void setup()
{
  pinMode(NW_pin, OUTPUT);
  pinMode(NE_pin, OUTPUT);
  pinMode(SE_pin, OUTPUT);
  pinMode(SW_pin, OUTPUT);
  pinMode(RC_pin, INPUT);

  test_all();
}

void test_all()
{
  blink_all();
  //fade_all();
}

void blink_all()
{
  blink(NW_pin, 500, 200);
  delay(10);
  blink(NE_pin, 500, 200);
  delay(20);
  blink(SE_pin, 500, 200);
  delay(20);
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

void displayPattern(unsigned long pattern, unsigned long pauseMsec=20)
{
  const byte *b = reinterpret_cast<byte const *>(&pattern);
  analogWrite(NW_pin, b[0]);
  analogWrite(NE_pin, b[1]);
  analogWrite(SE_pin, b[2]);
  analogWrite(SW_pin, b[3]);
  delay(pauseMsec);
}

void police()
{
  displayPattern(0xFFFF0000);
  displayPattern(0);
  displayPattern(0xFFFF0000);
  displayPattern(0);
  displayPattern(0xFFFF0000);
  displayPattern(0);
  displayPattern(0xFFFF0000);
  displayPattern(0);

  displayPattern(0x0000FFFF);
  displayPattern(0);
  displayPattern(0x0000FFFF);
  displayPattern(0);
  displayPattern(0x0000FFFF);
  displayPattern(0);
  displayPattern(0x0000FFFF);
  displayPattern(0);
}

byte getMode()
{
  static const float modeLength = (MAXPULSE - MINPULSE) / MODES;
  unsigned int CH1 = pulseIn(RC_pin, HIGH);
  unsigned int pulseLength = CH1;
  byte lightMode=0;
  if(CH1<minPulse)minPulse=CH1;
  if(CH1>maxPulse)maxPulse=CH1;
  if((maxPulse-minPulse)>FLUCTUATION)
  {
    if ((pulseLength) >= MINPULSE)
    {
      pulseLength -= MINPULSE;
    }
    else
    {
      pulseLength = 0;
    }

    lightMode = round((pulseLength) / modeLength);
    if (lightMode > MODES) lightMode = MODES;
    minPulse=CH1;
    maxPulse=CH1;
  }
  return lightMode;
}

void loop()
{
  byte mode = getMode();
  if (mode==0)
    police();
  else if (mode==1) 
    blink_all();
  else
    fade_all();
}
