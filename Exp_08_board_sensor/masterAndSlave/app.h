#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <SPI.h>

#include "SPI_anything.h"
#include "board.h"

#define SLAVE_LOOP_ITERATIONS (1000)

class App{
  protected:
    static const int bigBuffSize         = 20, // enough space for a long string
                     slaveProcessingTime = 5000; // millisecs

    void printSendCount() const;
    byte transferAndWait (const byte what) const;

    typedef struct DataStruct{
      char c0, 
           c1, 
           c2;
      int  i0;
    };

  public:
    static const byte nullChar =  '#';
    static const int pauseBetweenSends   = 10;  // microseconds
    
    App();
    virtual void loop() = 0;
    virtual void SPI_ISR () =0;
    virtual void serialEvent();
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

    Board *board;
    
    void fillStruct(byte inCar);
    void printOutData() const;
    void newBoard();
    
  public:
    SlaveApp();
    void loop();
    virtual void SPI_ISR ();    
    virtual void serialEvent();
};

#endif
