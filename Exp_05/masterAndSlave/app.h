#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <SPI.h>
#include "SPI_anything.h"

class App{
  protected:
    static const int pauseBetweenSends   = 10,  // microseconds
                     bigBuffSize         = 20, // enough space for a long string
                     slaveProcessingTime = 1000; // millisecs
    static const byte nullChar =  '#';

    void outgointMsg(char* buf) const;
    byte transferAndWait (const byte what) const;

  public:
    App();
    virtual void loop() = 0;
    virtual void SPI_ISR () =0;
};

 class MasterApp: public App{
  protected:
    char nextChar2Send() const;
  public:
    MasterApp();
    void loop();
    virtual void SPI_ISR (){}
};

class SlaveApp: public App{
  protected:
    volatile byte command = 0;
    volatile byte flag = 0;
    volatile char outChar = nullChar;
    
  public:
    SlaveApp();
    void loop();
    virtual void SPI_ISR ();    
};

#endif
