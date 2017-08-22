#include "slaveTest.h"

testS slaveApp::nextTS(){
  static uint8_t count = 0;
  testS ts = {count,count++,micros(),micros()/7.0};
  return ts;
}

void slaveApp::setT0(){
  startTime = micros();
}

slaveApp::slaveApp(int qLed){
  //q = new Q<testS>(nullTS,qLed);
  hbLED = qLed;
  pinMode(hbLED,OUTPUT);
}

// max time = 6 us !!
slaveApp::loop(){
  static uint32_t count = 1;
  static double tTotal = 0.0,
                maxT = 0;
  static const char chars[] = {nullChar, 'i', 'b'};
  static uint8_t ind = 0;
  uint8_t inFlag = chars[ind];
  ind = (ind+1) % 3;
  
  uint32_t t0 = micros();
  switch  (inFlag){
  case nullChar:
    break;
  case 'i':
    lim = u32Size;
    sendI = 0;
    setT0();
    outPtr = (uint8_t*)&startTime; 
    break;
  case 'b':
    lim = u32Size;
        sendI = 0;
    outPtr = (uint8_t*)&boardId; 
    break;
  default:
    lim = tsSize;
    sendI = 0;
    outgoingTS = nextTS();
    outPtr = (uint8_t*) &outgoingTS;
    break;
  }
  tTotal += (micros()-t0);
  maxT = max(maxT,tTotal/count);
  Serial.print("avg time: ");
  Serial.print(tTotal/count++);
  Serial.print("   Max time: ");
  Serial.println(maxT);
  digitalWrite(hbLED,!digitalRead(hbLED));
  delay (100);
  
}
slaveApp::slaveSPI_ISR(){
  // caled when a full byte has arrived, and master is waiting some microseconds
  inFlag  = SPDR;
  SPDR = (sendI<lim) ? (*(outPtr+sendI++)) : nullChar;
}

