#include "app.h"

SlaveApp::SlaveApp(): App(){
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // now turn on interrupts
  SPI.attachInterrupt();  
  while(!Serial);
  Serial.println("Slave");
}

void SlaveApp::loop(){
  //noInterrupts();
  if (flag){
    Serial.print("Command: ");
    Serial.println((char)command);
    Serial.print("Sending: ");
    //Serial.print("outchar: ");
    Serial.println(outChar);
    flag--;
  }
  //interrupts();
}

void SlaveApp::fillStruct(byte inCar){
  outData.c2 = outData.c1 = outData.c0 = (char)(inCar -32);
}

void SlaveApp::SPI_ISR(){
  if (SPDR != nullChar) {
    command = SPDR;
  }
  outChar =  char(command-32);
  SPDR = outChar;
  flag++;
}

