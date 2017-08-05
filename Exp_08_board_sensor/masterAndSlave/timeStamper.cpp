#include "timeStamper.h"

TimeStamper::TimeStamper(timeStamp_t tInit) : t0(tInit){}
    
timeStamp_t TimeStamper::getTimeStamp() const{
  return (timeStamp_t) (micros() - t0);
}

