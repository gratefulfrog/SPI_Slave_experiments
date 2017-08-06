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
  board->showQSize();
}

void SlaveApp::fillStruct(byte inCar){
  static boardID outBID = board->getGUID();
  static timeValStrut_t outTVS;
  static unsigned long time0;
  sendI = 0;
  //flag++;
  
  switch (inCar){
    case 't':
      lim = sizeof(unsigned long);
      time0 = micros();
      outPtr = (byte *) &time0;
      break;
    case 'i':
      lim = sizeof (boardID);
      outPtr = (byte *) &outBID;
      break;
    default:
      timeValStrut_t *tempTVS = board->pop(); 
      if(tempTVS){
        lim = sizeof (timeValStrut_t);
        outTVS.id = tempTVS->id;
        outTVS.t  = tempTVS->t;
        outTVS.v  = tempTVS->v;
        outPtr = (byte *) &outTVS;
      }
      else{
        lim = 0;
        outPtr = NULL;
      }
      break;
  } 
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
  board->loop(); //serialEvent();
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

