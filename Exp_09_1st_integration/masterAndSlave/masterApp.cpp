#include "app.h"

char MasterApp::nextChar2Send() const{
  static byte nextIndex = 0;
  const static char letters[] = "tia"; //bcdefghijklmnopqrstuvwxyz";
  char res = letters[nextIndex];
  nextIndex = 0;  // set this to 1 to send an 'a'
  return res;
}


/*
char MasterApp::nextChar2Send() const{
  static byte nextIndex = 0;
  const static char letters[] = "abcdefghijklmnopqrstuvwxyz";
  char res = letters[nextIndex++];
  nextIndex = nextIndex == 26 ? 0 : nextIndex;
  return res;
}
*/

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

void MasterApp::readReply(char command, char nextCommand){
  boardID inBoardID;
  unsigned long slaveTime;
  timeValStrut_t inTVS;
  
  switch (command){
    case 't':
      // set time to zero!
      SPI_readAnything_reprime(slaveTime, (byte)nextCommand);
      printReply(slaveTime, true);
      break;
    case 'i':
      SPI_readAnything_reprime(inBoardID, (byte)nextCommand);
      printReply(inBoardID,false);
      break;
    default:      
      SPI_readAnything_reprime(inTVS, (byte)nextCommand);
      printReply(inTVS);
      break;
  }
}

void MasterApp::loop(){  
  printSendCount();

   // print outgoing character that was just sent
  Serial.print("Sent: ");
  Serial.println(outgoing);
  Serial.print("Received: ");

  char nextOutgoing  = nextChar2Send();
  
  // enable Slave Select
  digitalWrite(SS, LOW);   
  delayMicroseconds (pauseBetweenSends);
  readReply(outgoing,nextOutgoing);
  outgoing = nextOutgoing;
   
  //SPI_readAnything_reprime(inData, (byte) outgoing);
  
  // disable Slave Select
  digitalWrite(SS, HIGH);
  /*
  Serial.print("inData.c0: ");
  Serial.println(inData.c0);
  Serial.print("inData.c1: ");
  Serial.println(inData.c1);
  Serial.print("inData.c2: ");
  Serial.println(inData.c2);
  Serial.print("inData.i0: ");
  Serial.println(inData.i0);
  */
  delay (slaveProcessingTime);
}


