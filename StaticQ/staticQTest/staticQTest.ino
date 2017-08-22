
#include <Arduino.h>
#include "staticQ.h"
#include "defs.h"
#include "slaveTest.h"

const unsigned int qSize = 10;

const int led = 3;

Q<testS> *q;
slaveApp *app;

void show(testS ts){
  Serial.print("bid: ");
  Serial.println(ts.bid);
  Serial.print("cid: ");
  Serial.println(ts.cid);
  Serial.print("t  : ");
  Serial.println(ts.t);
  Serial.print("v  : ");
  Serial.println(ts.v);
}

void setup(){
  Serial.begin(115200);
  while(!Serial);
  app = new slaveApp(led);
}

void loop(){
  app->loop();
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
