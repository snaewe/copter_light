#include <Arduino.h>

static int8_t step = 0;
static const byte MAX_STEPS=22;

static const unsigned long pattern[][MAX_STEPS] PROGMEM =
{
  { 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000, 0, 0xFFFF0000,0, 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF, 0, 0x0000FFFF, 0},
  { 0xff000000, 0xffff0000, 0x00ffff00, 0x0000ffff, 0x000000ff, 0},
  { 0, 0x1010, 0x1010, 0x2020, 0x2020, 0x4040, 0x4040, 0x8080, 0x8080, 0xffff, 0xffff, 0, 0x10100000, 0x10100000, 0x20200000, 0x20200000, 0x40400000, 0x40400000, 0x80800000, 0x80800000, 0xffff0000, 0xffff0000 },
  { 0xff00ff00,0,0,0,0,0x00ff00ff,0,0,0,0 },
};
static const byte NUM_PATTERNS = sizeof(pattern) / sizeof(pattern[0]);

void pattern_advance()
{
  ++step;
  if(step==MAX_STEPS)
  {
    step=0;
  }
}

unsigned long pattern_get(int8_t num)
{
  return pgm_read_dword(&pattern[((num>0) && (num<NUM_PATTERNS-1)) ? num : 0][step]);
}

int8_t pattern_count() { return NUM_PATTERNS; }
