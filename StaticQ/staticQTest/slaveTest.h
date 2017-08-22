#ifndef SLAVE_TEST_H
#define SLAVE_TEST_H

#include <Arduino.h>
#include "defs.h"
#include "staticQ.h"


class slaveApp{
 protected:
  static const uint8_t u32Size = sizeof(unsigned long),
                       tsSize = sizeof(testS);
  uint32_t startTime;
  unsigned long boardId = 1;  
           
  testS outgoingTS;
  Q<testS> *q;
  int hbLED;
  volatile byte inFlag,
                sendI,
                lim,
                *outPtr;
  volatile bool fired = false;
  testS nextTS();
  void setT0();
  
 public:
  slaveApp(int qLed);

  void loop();

  void slaveSPI_ISR();
};

#endif
