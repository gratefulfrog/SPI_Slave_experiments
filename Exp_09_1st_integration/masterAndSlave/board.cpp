#include "board.h"

const timeValStruct_t Board::nullReturn = {Sensor::nullSensorID,0,0};
    

Board::Board(boardID iid, 
             int nbSen, 
             Sensor ** sVec) :  nbDataGets(OUTPUT_BURST_LENGTH),
                                guid(iid), 
                                nbSensors(nbSen) {
  sensorVec = sVec;
  q = new Q<timeValStruct_t>;
  ts = new TimeStamper(micros());
}

void Board::updateSensorData(){
  static byte j = 0;
  timeValStruct_t *tvs = new timeValStruct_t;

  //Serial.println((long)sensorVec[j]);
  sensorVec[j]->getValue(*tvs);
  tvs->t = ts->getTimeStamp();
  noInterrupts();
  q->push(tvs);
  interrupts();
  j = (j+1) % nbSensors;
}


void Board::getData(){
  for (int i=0;i< nbDataGets;i++){
    timeValStruct_t *tvs = q->pop();
    if (tvs){   // we got one!!
      Sensor::serialPrintTVS(*tvs);
      delete tvs;
    }
    else{
      break;
    }
  }
  Serial.println("EOQ !");
}
boardID Board::getGUID() const{
  //Serial.print("Board GUID: ");
  //Serial.println(guid);
  return guid;
}

void Board::loop(){  
   if (!counter){
    updateSensorData();
  }
  counter = (counter + 1) % SLAVE_LOOP_ITERATIONS;
  
  /*
  if (incomingFlag){
    noInterrupts();
    processCommand();
    incomingFlag = false;
    interrupts();
  } 
   */
}

void Board::sendSPI(boardID id){
  Serial.print("Board ID: ");
  Serial.println(id);
}

void sendSPI(timeValStruct_t &tvs){
  Sensor::serialPrintTVS(tvs);
}

void Board::processCommand(){
  switch (incomingChar){
    case 'i':  // return ID
      sendSPI(getGUID());
      break;
    case 'a' :  // get data block
      getData();
      break;
  }
}

void Board::serialEvent(){
  if (Serial.available()) {
    incomingChar = (char)Serial.read();
    incomingFlag = true;
  }
} 
timeValStruct_t *Board::pop(){
  noInterrupts();
  timeValStruct_t *tvs = q->pop();
  interrupts();
  static timeValStruct_t res;
  static timeValStruct_t *resPtr;
  if (tvs){
    res.id = tvs->id;
    res.t  = tvs->t;
    res.v  = tvs->v;
    delete tvs;
    resPtr = &res;
  }
  else {
    resPtr = NULL;
  }
  return  resPtr;
}

void Board::showQSize() const{
  Serial.print("Q nbObjects: ");
  Serial.println(q->qNbObjects());
}

void Board::setT0(timeStamp_t time0){
  ts->setTime0(time0);
}

void Board::clearQ(){
  while(pop());
}

