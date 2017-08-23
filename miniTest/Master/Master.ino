
#include <Arduino.h>
#include <SPI.h>

const int timeout = 100, //us
          loopPause = 5; //ms
const uint32_t showFrequency = 2000;

uint8_t b[4];

SPISettings settings;

void setup(){
  Serial.begin(115200);
  SPI.begin();
  Serial.println("Master");  

  settings = SPISettings (2000000, MSBFIRST, SPI_MODE0);
}

void oneShot(bool once){
  uint32_t valIn = 0;
  uint8_t *p = (uint8_t*)&valIn,
          in;
  static uint32_t lastValin = 0;
  
  if (once){
    SPI.beginTransaction (settings);
    digitalWrite (SS, LOW); 
    SPI.transfer ('a');  
    delayMicroseconds(timeout);
    digitalWrite (SS, HIGH);
    SPI.endTransaction ();  
  }
  
  for (int i=0;i<3;i++){
    SPI.beginTransaction (settings);
    digitalWrite (SS, LOW);
    //delayMicroseconds(timeout);
    in = SPI.transfer ('#');  
    delayMicroseconds(timeout);
    p[i] =  in;
    digitalWrite (SS, HIGH);
    SPI.endTransaction (); 
  }

  SPI.beginTransaction (settings);
  digitalWrite (SS, LOW); 
  //delayMicroseconds(timeout);
  p[3] = SPI.transfer ('a');  
  delayMicroseconds(timeout);
  digitalWrite (SS, HIGH);
  SPI.endTransaction (); 
  //if(!(valIn % showFrequency)){
    Serial.print("valIn: ");
    Serial.println(valIn);
  //}
  if (lastValin && valIn != lastValin+1){
    Serial.println("Error!!! out of sequence");
    for (int i=0;i<4;i++){
      p = (uint8_t*)&valIn,
      Serial.println(String("p[") + String(i) + String("] : ") + String(p[i]));
      //Serial.println(String("last val in : ") + String(lastValin));
    }
    while(1);
  }
  lastValin = valIn;
}

void loop(){
  static bool once = true;
  oneShot(once);
  if (once){
    once = false;
  }
  
  delay(loopPause);
}
