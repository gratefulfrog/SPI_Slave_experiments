#ifndef DEFS_H
#define DEFS_H

#include <Arduino.h>

struct testS {
  uint8_t  bid,
    cid;
  uint32_t t;
  float    v;
};

extern const struct testS nullTS;

const char nullChar = '#';

#endif
