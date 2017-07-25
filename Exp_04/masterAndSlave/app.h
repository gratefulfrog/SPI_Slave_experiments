#ifndef APP_H
#define APP_H

#include <SPI.h>
#include "SPI_anything.h"

class App{
  protected:
    const int pauseBetweenSends   = 10,  // microseconds
              bigBuffSize         = 100, // enough space for a long string
              slaveProcessingTime = 2000; // millisecs

    typedef struct dataStruct{
      byte a;
      int b;
      long c;
    };

  volatile dataStruct data;

  void outgointMsg(char* buf) const{
    // puts a char[] into the arg buf,
  
    static int sendCount = 0;
    String s = "Send: " + String(sendCount++) + " !\n";
    s.toCharArray(buf,s.length()+1);
  }
  byte transferAndWait (const byte what) const;

  public:
    App();
    virtual void loop() = 0;
    virtual void SPI_ISR () =0;
};

 class MasterApp: public App{
  public:
    MasterApp();
    void loop();
    virtual void SPI_ISR (){}
};

class SlaveApp: public App{
  protected:
    volatile byte command = 0;
    
  public:
    SlaveApp();
    void loop();
    virtual void SPI_ISR ();    
};

#endif
