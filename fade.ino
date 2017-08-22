void fade_all()
{
  fadeUp(pin[NW], 0, 255, 50);
  fadeUp(pin[SW], 0, 255, 50);
  fadeUp(pin[SE], 0, 255, 50);
  fadeUp(pin[NE], 0, 255, 50);
  delay(500);
  fadeDown(pin[NE], 255, 0, 50);
  fadeDown(pin[SE], 255, 0, 50);
  fadeDown(pin[SW], 255, 0, 50);
  fadeDown(pin[NW], 255, 0, 50);
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

