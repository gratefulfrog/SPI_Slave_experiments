
#include <Arduino.h>
#include <SPI.h>
#include "staticQ.h"
#include "defs.h"
#include "slaveTest.h"

const unsigned int qSize = 10;

const int led = 3;

Q<testS> *q;
slaveApp *app;

void setup(){
  Serial.begin(115200);
  //while(!Serial);

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // now turn on interrupts
  SPI.attachInterrupt();
  app = new slaveApp(led);
  
}

void loop(){
  app->loop();
}
// SPI interrupt routine
ISR (SPI_STC_vect){
  app->slaveSPI_ISR();
}

/*
void setup(){
  Serial.begin(115200);
  while(!Serial);
  q  = new Q<struct testS>(nullTS, led);
}

void loop(){
  static uint8_t count = 0;
  testS ts = {count,count++,micros(),micros()/7.0};
  q->push(ts);
  if(digitalRead(led)){
    Serial.println("popping...");
    delay(100);
    testS res = q->pop();
    while(res.bid !=255){
      show(res);
      res = q->pop();
    }
  }
  //delay(500);
}
*/
