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
  static unsigned long time0;
  static unsigned int maxQ =0;
  if (init){
    board->loop();
    if(board->getQSize() > maxQ){
      maxQ = board->getQSize();
      Serial.print("Max Q size: ");
      Serial.println(maxQ);
    }
  }
  else if (command == 't'){
    time0 =  micros();
    board->setT0(time0);
    board->clearQ();

    lim = sizeof(unsigned long);
    outPtr = (byte *) &time0;
    Serial.print("init t0: ");
    Serial.println(time0);
    init=true;
  }
}

void SlaveApp::fillStruct(byte inCar){
  static boardID outBID = board->getGUID();
  static timeValStruct_t outTVS;
  sendI = 0;
  
  switch (inCar){
    case 't':
      init = false;
      break;
    case 'i':
      lim = sizeof (boardID);
      outPtr = (byte *) &outBID;
      break;
    default:
      lim = sizeof (timeValStruct_t);
      timeValStruct_t *tempTVS = board->pop(); 
      if(tempTVS){
        outTVS.id = tempTVS->id;
        outTVS.t  = tempTVS->t;
        outTVS.v  = tempTVS->v;
        outPtr = (byte *) &outTVS;
      }
      else{
        outPtr = (byte *) &Board::nullReturn;
      }
      break;
  } 
}

void SlaveApp::SPI_ISR(){
  if (SPDR != nullChar) {
    command = SPDR;
    fillStruct(command);  // will take some time, but can't see how to make it faster because outPtr must be set here!
  }
  
  if (sendI++<lim){
    SPDR = (*outPtr++);
  }
}

void SlaveApp::newBoard(){
  const boardID bid = 0;
  const byte nbSensors = 2;
  const sensorID_t vibSID = 0,
                   lightSID = 1;

  Sensor **sVec = new Sensor*[nbSensors];
  
  sVec[vibSID] = new VibrationSensor(vibSID);
  sVec[lightSID] = new LightSensor(lightSID);
 
  board = new Board(bid,nbSensors, sVec);
}

