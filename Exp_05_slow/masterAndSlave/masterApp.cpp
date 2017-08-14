#include "app.h"


char MasterApp::nextChar2Send() const{
  static byte nextIndex = 0;
  const static char letters[] = "abcdefghijklmnopqrstuvwxyz";
  char res = letters[nextIndex++];
  nextIndex = nextIndex == 26 ? 0 : nextIndex;
  return res;
}

MasterApp::MasterApp():App(){
  SPI.begin();
  
  Serial.println("Master");

  // prime the pump
  char buff[bigBuffSize];
  outgointMsg(buff);
  Serial.print(buff);
  
  char outgoing  = nextChar2Send();

  // print outgoing character
  Serial.print("Sent: ");
  Serial.println(outgoing);

  //SPI.beginTransaction (SPISettings (1000, MSBFIRST, SPI_MODE0));  // 1Khz clock
  // enable Slave Select
  digitalWrite(SS, LOW);
  delayMicroseconds(20);    

  
  // send outgoing character, ignore resonse, this primes the pump
  transferAndWait (outgoing);  
  
  // disable Slave Select
  digitalWrite(SS, HIGH);
  //SPI.endTransaction();
  delayMicroseconds(20);
}

void MasterApp::loop(){  
  char outgoing  = nextChar2Send();
  Serial.print("Received: ");

  //SPI.beginTransaction (SPISettings (1000, MSBFIRST, SPI_MODE0));  // 1Khz clock
  // enable Slave Select
  digitalWrite(SS, LOW);   
  delayMicroseconds(20); 

  
  // send next outgoing character, receive resonse to previous send
  Serial.println((char)transferAndWait (outgoing));  

  // disable Slave Select
  digitalWrite(SS, HIGH);
  //SPI.endTransaction();

  delay (slaveProcessingTime);
  
  // print heading and send count
  char buff[bigBuffSize];
  outgointMsg(buff);
  Serial.print(buff);
  
   // print outgoing character that was just sent
  Serial.print("Sent: ");
  Serial.println(outgoing);
}


