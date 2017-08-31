#include "fsm.hpp"
#include "globals.hpp"

static int max_count = 0;

void on_enter_light_off()
{
  current = -1;
}

void increase_current() { if (current<max_count) ++current; }
void decrease_current() { if (current!=-1) --current; }

State Light_Off(&on_enter_light_off, 0);
State High(0, 0);
State Neutral(0,0);
State Low(0,0);

Fsm fsm(&Light_Off);

void setup_fsm(int maxCount)
{
  /*
   * Light_Off    + Upper    => High
   * High         + Middle   => Neutral
   * Neutral      + Upper    => High
   * Neutral      + Lower    => Low
   * Low          + Middle   => Neutral
   */
  max_count = maxCount;
  fsm.add_transition(&Light_Off, &High, Upper, &on_enter_light_off);
  fsm.add_transition(&High, &Neutral, Middle, 0);
  fsm.add_transition(&Neutral, &High, Upper, &increase_current);
  fsm.add_transition(&Neutral, &Low,  Lower, &decrease_current);
  fsm.add_transition(&Low, &Neutral, Middle, 0);
}
