#include "utilities.h"

using namespace std;

/*******************************/
// some arduino compatible stuff
/*******************************/

void  delay(time_t millisecs){
  delayFull(millisecs/1000.0,0);
}

void  delayMicroseconds(long microsecs){
  struct timespec delayTime = {0,microsecs*1000};
  nanosleep(&delayTime,NULL);
}

void delaySeconds(time_t secs){
  struct timespec delayTime = {secs,0};
  nanosleep(&delayTime,NULL);
}

void delayFull(time_t secs, long micros){
  struct timespec delayTime = {secs,micros};
  nanosleep(&delayTime,NULL);
}

