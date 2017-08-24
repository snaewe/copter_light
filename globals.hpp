#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <Arduino.h>
#include <Fsm.h>

extern int8_t current;
extern Fsm fsm;

#define MINPULSE 1000
#define MAXPULSE 2040

template <typename T> T clip(T val, T maxVal) { return (val>maxVal) ? maxVal : val;}

template <typename T> T limit(T x, T a, T b)
{
    return (x<a) ? a : (x>b) ? b : x;
}

inline long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif
