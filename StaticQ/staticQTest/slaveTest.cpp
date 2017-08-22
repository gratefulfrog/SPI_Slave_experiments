#include "slaveTest.h"

testS slaveApp::nextTS(){
  static uint8_t count = 0;
  testS ts = {count,count++,micros(),micros()/7.0};
  show(ts);
  return ts;
}

void slaveApp::setT0(){
  startTime = micros();
}

slaveApp::slaveApp(int qLed){
  //q = new Q<testS>(nullTS,qLed);
  hbLED = qLed;
  pinMode(hbLED,OUTPUT);
  Serial.println("Slave");
  fired =false;
  startTime = micros();
  boardId = 1;
  inFlag = nullChar;
  lim=0;
  sendI=0;
}

// max time = 6 us !!
void slaveApp::loop(){
  static uint32_t count = 0;

  //noInterrupts();
  switch  (inFlag){
  case nullChar:
    break;
  case 'i':
    lim = u32Size;
    sendI = 0;
    setT0();
    outPtr = (byte*)&startTime; 
    //Serial.print("outPtr: ");
    //Serial.println(startTime);
    inFlag = '#';
    break;
  case 'b':
    Serial.println("got a b!");
    lim = u32Size +2;
    sendI = 0;
    outPtr = (byte*)&boardId; 
    inFlag = '#';
    break;
  case 'a':
    lim = tsSize;
    sendI = 0;
    outgoingTS = nextTS();
    outPtr = (byte*) &outgoingTS;
    inFlag = '#';
    break;
   default:
    Serial.println(inFlag);
    break;
  }
  if (fired){
    digitalWrite(hbLED,!digitalRead(hbLED));
    fired=false;
  }
    //interrupts();
  /*
  tTotal += (micros()-t0);
  maxT = max(maxT,tTotal/count);
  Serial.print("avg time: ");
  Serial.print(tTotal/count++);
  Serial.print("   Max time: ");
  Serial.println(maxT);
 
  if(!count){
    digitalWrite(hbLED,!digitalRead(hbLED));
  }
  */
  count = (count+1)%20000; 
}
void slaveApp::slaveSPI_ISR(){
  // caled when a full byte has arrived, and master is waiting some microseconds
  inFlag  = SPDR;
  SPDR = (sendI++<lim) ? (*outPtr++) : nullChar;
  fired=true;
}

