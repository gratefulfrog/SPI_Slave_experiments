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
  noInterrupts();
  if (flag == 1){
    Serial.print("Command: ");
    Serial.println((char)command);
    Serial.print("Sending: ");
    Serial.println((char)outChar);
    flag = 0;
  }
  interrupts();
}

void SlaveApp::SPI_ISR(){
  command = SPDR;
  outChar =  char(command-32);
  SPDR = outChar;
  flag = 1;
}

