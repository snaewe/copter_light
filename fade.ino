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

