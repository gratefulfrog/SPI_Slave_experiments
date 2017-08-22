#ifndef SLAVE_TEST_H
#define SLAVE_TEST_H

#include <Arduino.h>
#include "defs.h"
#include "staticQ.h"


class slaveApp{
 protected:
  static const uint8_t u32Size = sizeof( uint32_t),
                       tsSize = sizeof(testS);
  uint32_t boardId = 1,
           startTime;  
           
  testS outgoingTS;
  Q<testS> *q;
  int hbLED;
  volatile uint8_t inFlag,
    sendI,
    lim,
    *outPtr;

  testS nextTS();
  void setT0();
  
 public:
  slaveApp(int qLed);

  loop();

  slaveSPI_ISR();
};

#endif
