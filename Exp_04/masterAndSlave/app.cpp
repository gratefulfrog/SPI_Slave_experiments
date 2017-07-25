#include "app.h"

App::App(){
  Serial.begin (115200);
}

byte App::transferAndWait (const byte what) const{
  byte res = SPI.transfer (what);
  delayMicroseconds (pauseBetweenSends);
  return res;
} 
