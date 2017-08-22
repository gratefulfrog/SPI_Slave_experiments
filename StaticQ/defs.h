#ifndef DEFS_H
#define DEFS_H

#include <Arduino.h>

struct testS {
  uint8_t  bid,
    cid;
  uint32_t t;
  float    v;
};

const testS nullTS = {255,0,0,0};

const unsigned long pauseBetweenSends = 20; //us

const char nullChar = '#';

extern void show(const testS &ts);

#endif
