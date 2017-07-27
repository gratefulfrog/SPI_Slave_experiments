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

  // send outgoing character, ignore resonse, this primes the pump
  transferAndWait (outgoing);  
  
  // disable Slave Select
  digitalWrite(SS, HIGH);
}

void MasterApp::loop(){  
  char outgoing  = nextChar2Send();
  Serial.print("Received: ");
  
  // enable Slave Select
  digitalWrite(SS, LOW);    
 
  // send next outgoing character, receive resonse to previous send
  Serial.println((char)transferAndWait (outgoing));  

  // disable Slave Select
  digitalWrite(SS, HIGH);

  delay (slaveProcessingTime);
  
  // print heading and send count
  char buff[bigBuffSize];
  outgointMsg(buff);
  Serial.print(buff);
  
   // print outgoing character that was just sent
  Serial.print("Sent: ");
  Serial.println(outgoing);
}


