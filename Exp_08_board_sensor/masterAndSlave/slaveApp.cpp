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

  newBoard();
}

void SlaveApp::loop(){
  board->loop();
}

/*
void SlaveApp::loop(){
  //noInterrupts();  not needed because master gives enough time between coms for this to happen;
  if (flag){
    Serial.print("Command: ");
    Serial.println((char)command);
    Serial.print("Sending: ");
    printOutData();
    flag--;
  }
}
*/

void SlaveApp::fillStruct(byte inCar){
  outData.c2 = outData.c1 = outData.c0 = (char)(inCar -32);
  outData.i0 = counter++;
  outPtr = (byte *) &outData;
  sendI = 0;
  flag++;
}

void SlaveApp::SPI_ISR(){
  if (SPDR != nullChar) {
    command = SPDR;
    fillStruct(command);
  }
  
  if (sendI++<lim){
    SPDR = (*outPtr++);
  }
}

void SlaveApp::printOutData() const{
  Serial.print("outData.c0: ");
  Serial.println(outData.c0);
  Serial.print  ("outData.c1: ");
  Serial.println(outData.c1);
  Serial.print  ("outData.c2: ");
  Serial.println(outData.c2);
  Serial.print  ("outData.i0: ");
  Serial.println(outData.i0);
}

void SlaveApp::serialEvent(){
  // call board serial.event
  board->serialEvent();
}

void SlaveApp::newBoard(){
  const boardID bid = 0;
  const byte nbSensors = 2;
  const sensorID_t vibSID = 0,
                   lightSID = 1;
  TimeStamper *t = new TimeStamper(micros());
  
  Sensor **sVec = new Sensor*[nbSensors];
  
  sVec[vibSID] = new VibrationSensor(vibSID,t);
  sVec[lightSID] = new LightSensor(lightSID,t);
 
  board = new Board(bid,nbSensors, sVec);
  
}

