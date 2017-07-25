#include "app.h"

MasterApp::MasterApp():App(){
  SPI.begin ();

  // Slow down the master a bit
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
  
  Serial.println("Master");
}
void MasterApp::loop(){
  char buff[bigBuffSize];
  outgointMsg(buff);

  Serial.print(buff);

  byte a, b, c, d;
  
  // enable Slave Select
  digitalWrite(SS, LOW);    

  transferAndWait ('a');  // add command
  transferAndWait (10);
  a = transferAndWait (17);
  b = transferAndWait (33);
  c = transferAndWait (42);
  d = transferAndWait (0);

  // disable Slave Select
  digitalWrite(SS, HIGH);

  Serial.println ("Adding results:");
  Serial.println (a, DEC);
  Serial.println (b, DEC);
  Serial.println (c, DEC);
  Serial.println (d, DEC);
  
  // enable Slave Select
  digitalWrite(SS, LOW);   

  transferAndWait ('s');  // subtract command
  transferAndWait (10);
  a = transferAndWait (17);
  b = transferAndWait (33);
  c = transferAndWait (42);
  d = transferAndWait (0);

  // disable Slave Select
  digitalWrite(SS, HIGH);

  Serial.println ("Subtracting results:");
  Serial.println (a, DEC);
  Serial.println (b, DEC);
  Serial.println (c, DEC);
  Serial.println (d, DEC);
  
  delay (slaveProcessingTime);  // for testing  
}


