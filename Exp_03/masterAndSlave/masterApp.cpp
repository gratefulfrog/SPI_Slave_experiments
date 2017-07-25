#include "app.h"

MasterApp::MasterApp():App(){
  SPI.begin ();
  
  data.a = 42;
  data.b = 32000;
  data.c = 100000;
  Serial.println("Master");
}
void MasterApp::loop(){
  char buff[bigBuffSize];
  outgointMsg(buff);

  Serial.print(buff);
  digitalWrite(SS, LOW);    // SS is pin 10
  SPI_writeAnything (data);
  digitalWrite(SS, HIGH);
  delay (slaveProcessingTime);  // for testing  
  
  data.c++;
}

