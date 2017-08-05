#ifndef TIMESTAMPER_H
#define TIMESTAMPER_H

#include <Arduino.h>

typedef unsigned long timeStamp_t;
  
class TimeStamper {
  protected:
    const timeStamp_t t0;

  public:
    TimeStamper(timeStamp_t tInit);
    timeStamp_t getTimeStamp() const;
};


#endif
