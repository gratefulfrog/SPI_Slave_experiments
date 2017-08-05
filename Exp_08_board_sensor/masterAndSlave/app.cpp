#include "app.h"

App::App(){
  Serial.begin (115200);
}

void App::printSendCount() const{
  static int sendCount = 0;
  String s = "Send: " + String(sendCount++) + "\n";
  Serial.print(s);
}

byte App::transferAndWait (const byte what) const{
  byte res = SPI.transfer (what);
  delayMicroseconds (pauseBetweenSends);
  return res;
} 

void App::serialEvent(){
  Serial.println("App.serialEvent...");
}

