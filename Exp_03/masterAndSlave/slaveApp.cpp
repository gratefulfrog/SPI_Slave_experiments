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

void SlaveApp::loop(){
  if (haveData){
     Serial.println ((int) data.a);
     Serial.println (data.b);
     Serial.println (data.c);
     Serial.println ();
     haveData = false;
  }
}

void SlaveApp::SPI_ISR(){
  SPI_readAnything_ISR (data);
  haveData = true;
}

