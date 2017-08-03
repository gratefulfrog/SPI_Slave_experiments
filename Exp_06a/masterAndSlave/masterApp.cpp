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
  /*
  // prime the pump
  char buff[bigBuffSize];
  outgointMsg(buff);
  Serial.print(buff);
  
  //char outgoing  = nextChar2Send();

  // print outgoing character
  //Serial.print("Sent: ");
  //Serial.println(outgoing);
  
  // enable Slave Select
  //digitalWrite(SS, LOW);    

  // send outgoing character, ignore resonse, this primes the pump
  //transferAndWait (outgoing);  
  
  // disable Slave Select
  //digitalWrite(SS, HIGH);
  */

}

void MasterApp::loop(){  
  char outgoing  = nextChar2Send();
  char buff[bigBuffSize];
  outgointMsg(buff);
  Serial.print(buff);
  
   // print outgoing character that was just sent
  Serial.print("Sent: ");
  Serial.println(outgoing);
  Serial.print("Received: ");
  
  // enable Slave Select
  digitalWrite(SS, LOW);   
  delayMicroseconds (pauseBetweenSends);
   
  //SPI_readAnything_reprime(inData,outgoing);
  // send next outgoing character, receive resonse to previous send
  //Serial.println((char)transferAndWait (outgoing));  
  char ignored = transferAndWait(outgoing);
  SPI_readAnything(inData);
  /*       c1 = transferAndWait(nullChar),
       c2 = transferAndWait(nullChar),
       c3 = transferAndWait(nullChar);
  */
  // disable Slave Select
  digitalWrite(SS, HIGH);
  
  Serial.print("inData.c0: ");
  Serial.println(inData.c0);
  Serial.print("inData.c1: ");
  Serial.println(inData.c1);
  Serial.print("inData.c2: ");
  Serial.println(inData.c2);
  /*
  Serial.print("Ignored: ");
  Serial.println(ignored);
  Serial.print("c1: ");
  Serial.println(c1);
  Serial.print("c2: ");
  Serial.println(c2);
  Serial.print("c3: ");
  Serial.println(c3);
   */
  
  
  delay (slaveProcessingTime);
  
 
}


