
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

static const byte MAX_STEPS=11;
static const unsigned long pattern[][MAX_STEPS] =
{
#if 1
  { 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000},
  { 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF},
  { 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000},
  { 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF},
  { 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000},
  { 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF},

#else
  { 0xff000000, 0xffff0000, 0x00ffff00, 0x0000ffff, 0x000000ff, 0},
  { 0xff000000, 0xffff0000, 0x00ffff00, 0x0000ffff, 0x000000ff, 0},
  { 0xff000000, 0xffff0000, 0x00ffff00, 0x0000ffff, 0x000000ff, 0},
  { 0, 0x1010, 0x1010, 0x2020, 0x2020, 0x4040, 0x4040, 0x8080, 0x8080, 0xffff, 0xffff },
  { 0, 0x1010, 0x1010, 0x2020, 0x2020, 0x4040, 0x4040, 0x8080, 0x8080, 0xffff, 0xffff },
  { 0, 0x1010, 0x1010, 0x2020, 0x2020, 0x4040, 0x4040, 0x8080, 0x8080, 0xffff, 0xffff },
  { 0, 0x10100000, 0x10100000, 0x20200000, 0x20200000, 0x40400000, 0x40400000, 0x80800000, 0x80800000, 0xffff0000, 0xffff0000 },
  { 0, 0x10100000, 0x10100000, 0x20200000, 0x20200000, 0x40400000, 0x40400000, 0x80800000, 0x80800000, 0xffff0000, 0xffff0000 },
  { 0, 0x10100000, 0x10100000, 0x20200000, 0x20200000, 0x40400000, 0x40400000, 0x80800000, 0x80800000, 0xffff0000, 0xffff0000 },
#endif
};
static const byte NUM_PATTERNS = sizeof(pattern) / sizeof(pattern[0]);

static byte current = 0, step = 0;

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
  const byte *b = reinterpret_cast<byte const *>(&pattern[current][step]);
  analogWrite(NW_pin, b[0]);
  analogWrite(NE_pin, b[1]);
  analogWrite(SE_pin, b[2]);
  analogWrite(SW_pin, b[3]);
  b+=4;
  ++step;
  if(step==MAX_STEPS)
  {
    step=0;
    ++current;
    if (current == NUM_PATTERNS)
      current = 0;
  }
  else
    delay(50);
}
