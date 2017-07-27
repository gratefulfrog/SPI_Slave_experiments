#include "app.h"


char MasterApp::nextChar2Send() const{
  static byte nextIndex = 0;
  const static char letters[] = "abcdefghijklmnopqrstuvwxyz";
  char res = letters[nextIndex++];
  nextIndex = nextIndex == 26 ? 0 : nextIndex;
  return res;
}

MasterApp::MasterApp():App(){
  SPI.begin ();

  // Slow down the master a bit
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
  
  Serial.println("Master");

  // prime the pump
   char buff[bigBuffSize];
  outgointMsg(buff);
  Serial.print(buff);
  
  char outgoing  = nextChar2Send();

  // print outgoing character
  Serial.print("Sent: ");
  Serial.println(outgoing);
  
  // enable Slave Select
  digitalWrite(SS, LOW);    

  // send outgoing character, ignore resonse
  transferAndWait (outgoing);  
  // disable Slave Select
  digitalWrite(SS, HIGH);
  
}
void MasterApp::loop(){
  
  char outgoing  = nextChar2Send();
  
  // enable Slave Select
  digitalWrite(SS, LOW);    

  Serial.print("Received: ");
  // send outgoing character, receive resonse to previous send
  Serial.println((char)transferAndWait (outgoing));  

/*
  // send a null and recover response to previous send
  // note: this empty send allows for the next cycle to work
  Serial.print("Received: ");
  Serial.println((char)transferAndWait (nullChar));  
  */
  // disable Slave Select
  digitalWrite(SS, HIGH);

  delay (slaveProcessingTime);
  
  // print heading and send count
  char buff[bigBuffSize];
  outgointMsg(buff);
  Serial.print(buff);
   // print outgoing character
  Serial.print("Sent: ");
  Serial.println(outgoing);
  
  
}


