#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <SPI.h>

#include "SPI_anything.h"

struct DataStruct{
      char c0, 
           c1, 
           c2;
      int  i0;
    }__attribute__((__packed__));

class App{
  protected:
    static const int bigBuffSize         = 20, // enough space for a long string
                     slaveProcessingTime = 5000; // millisecs

    void printSendCount() const;
    byte transferAndWait (const byte what) const;

    
  public:
    static const byte nullChar =  '#';
    static const int pauseBetweenSends   = 10;  // microseconds
    
    App();
    virtual void loop() = 0;
    virtual void SPI_ISR () =0;
};

 class MasterApp: public App{
  protected:
    char nextChar2Send() const,
         outgoing;
    DataStruct inData;
    
  public:
    MasterApp();
    void loop();
    virtual void SPI_ISR (){}
};

class SlaveApp: public App{
  protected:
    volatile byte command = 0,
                  flag    = 0;
    volatile DataStruct outData;

    volatile byte *outPtr;
    static const int lim = sizeof outData;
    volatile int sendI,
                 counter;
    
    void fillStruct(byte inCar);
    void printOutData() const;
    
  public:
    SlaveApp();
    void loop();
    virtual void SPI_ISR ();    
};

#endif
