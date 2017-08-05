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
  while(!Serial);
  Serial.println("Master");

  /* prime the pump for the 1st character */
  outgoing  = nextChar2Send();
  digitalWrite(SS, LOW);   
  delayMicroseconds (pauseBetweenSends);
   
  transferAndWait(outgoing);  // ignore this reply!  
}

void MasterApp::loop(){  
  printSendCount();

   // print outgoing character that was just sent
  Serial.print("Sent: ");
  Serial.println(outgoing);
  Serial.print("Received: ");

  outgoing  = nextChar2Send();
  
  // enable Slave Select
  digitalWrite(SS, LOW);   
  delayMicroseconds (pauseBetweenSends);
   
  SPI_readAnything_reprime(inData, (byte) outgoing);
  
  // disable Slave Select
  digitalWrite(SS, HIGH);
  
  Serial.print("inData.c0: ");
  Serial.println(inData.c0);
  Serial.print("inData.c1: ");
  Serial.println(inData.c1);
  Serial.print("inData.c2: ");
  Serial.println(inData.c2);
  Serial.print("inData.i0: ");
  Serial.println(inData.i0);

  
  
  delay (slaveProcessingTime);
}


