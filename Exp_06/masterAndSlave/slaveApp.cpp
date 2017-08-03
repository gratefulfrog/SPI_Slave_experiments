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
  noInterrupts();
  if (flag == 1){
    Serial.print("Command: ");
    Serial.println((char)command);
    Serial.print("Sending: ");
    Serial.print("c0: ");
    Serial.println(outData.c0);
    Serial.print("c1: ");
    Serial.println(outData.c1);
    Serial.print("c2: ");
    Serial.println(outData.c2);
    flag = 0;
  }
  interrupts();
}

void SlaveApp::fillStruct(byte inCar){
  outData.c2 = outData.c1 = outData.c0 = (char)(inCar -32);
}

void SlaveApp::SPI_ISR(){
  //if (SPDR != nullChar) {
    command = SPDR;
    outChar =  char(command-32);
    //SPDR = outChar;
    fillStruct(command);
    SPI_writeAnything(outData);
    flag = 1;
  //}
}

