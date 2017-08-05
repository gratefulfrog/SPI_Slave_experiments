#include "board.h"

Board::Board(boardID iid, 
             int nbSen, 
             Sensor ** sVec) : nbLoopIterations(NB_LOOP_ITERATIONS), 
                                nbDataGets(OUTPUT_BURST_LENGTH),
                                guid(iid), 
                                nbSensors(nbSen) {
  sensorVec = sVec;
  q = new Q<timeValStrut_t>;
}

void Board::updateSensorData(){
  for (int i=0;i<nbLoopIterations;i++){
    for(int j=0;j<nbSensors;j++){
      timeValStrut_t *tvs = new timeValStrut_t;
      //Serial.println((long)sensorVec[j]);
      sensorVec[j]->getValue(*tvs);
      noInterrupts();
      q->push(tvs);
      interrupts();
      
    }
  }
}
void Board::getData(){
  for (int i=0;i< nbDataGets;i++){
    timeValStrut_t *tvs = q->pop();
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
  return guid;
}

void Board::loop(){  
  updateSensorData();
 
  if (incomingFlag){
    noInterrupts();
    processCommand();
    incomingFlag = false;
    interrupts();
  } 
   
}

void Board::sendSPI(boardID id){
  Serial.print("Board ID: ");
  Serial.println(guid);
}

void sendSPI(timeValStrut_t &tvs){
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
  

