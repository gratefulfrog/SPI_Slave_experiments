#include "app.h"

SlaveApp::SlaveApp(): App(){
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // now turn on interrupts
  SPI.attachInterrupt();  
  Serial.println("Slave");
}

void SlaveApp::SlaveApp::loop(){
   if (digitalRead (SS) == HIGH)
    command = 0;
}

void SlaveApp::SPI_ISR(){
  byte c = SPDR;
  switch (command){
  // no command? then this is the command
  case 0:
    command = c;
    SPDR = 0;
    break;
    
  // add to incoming byte, return result
  case 'a':
    SPDR = c + 15;  // add 15
    break;
    
  // subtract from incoming byte, return result
  case 's':
    SPDR = c - 8;  // subtract 8
    break;
  } 
}

