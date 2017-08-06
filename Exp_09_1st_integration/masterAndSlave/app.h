#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <SPI.h>

#include "SPI_anything.h"
#include "board.h"

struct assoc_t {
  char c;
  unsigned int size;
};

class App{
  protected:
    static const assoc_t msgAssoc[2] = {{'i', sizeof(boardID)}, 
                                        {'a', sizeof(sensorValue_t)}};
    
    static const int bigBuffSize         = 20, // enough space for a long string
                     slaveProcessingTime = 0; // millisecs  with 6 it is too slow for the slave and the slave overfills its q !

    void printSendCount() const;
    byte transferAndWait (const byte what) const;

    void printReply(unsigned long &v, boolean isTime);
    void printReply(timeValStruct_t &tvs);


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

    void readReply(char command, char nextCommand);
    
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
    unsigned int lim ;
    volatile int sendI,
                 counter;

    Board *board;

    volatile boolean init = false;
    
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
