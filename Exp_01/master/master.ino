// Written by Nick Gammon
// April 2011


#include <SPI.h>

const int pauseBetweenSends   = 10,  // microseconds
          bigBuffSize         = 100, // enough space for a long string
          slaveProcessingTime = 2000; // millisecs

void setup (void){
  Serial.begin (115200);
  //Serial.println ();
  
  digitalWrite(SS, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();

  // Slow down the master a bit
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
  
}  // end of setup

byte transferAndWait (const byte what){
  byte a = SPI.transfer (what);
  delayMicroseconds (pauseBetweenSends);
  return a;
} // end of transferAndWait

void outgointMsg(char* buf){
  // puts a char[] into the arg buf,
  // msg is "Hello, world: " plus a send counter

  static int sendCount = 0;
  String s = "Send: " + String(sendCount++) + " !\n";
  s.toCharArray(buf,s.length()+1);
}


void loop (void){

  byte a, b, c, d;

  char buff[bigBuffSize];
  outgointMsg(buff);
  
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

  Serial.print(buff);
  Serial.println ("Adding results:");
  Serial.println (a, DEC);
  Serial.println (b, DEC);
  Serial.println (c, DEC);
  Serial.println (d, DEC);

  delay (slaveProcessingTime);
  
  // enable Slave Select
  outgointMsg(buff);
  
  digitalWrite(SS, LOW);   

  transferAndWait ('s');  // subtract command
  transferAndWait (10);
  a = transferAndWait (17);
  b = transferAndWait (33);
  c = transferAndWait (42);
  d = transferAndWait (0);

  // disable Slave Select
  digitalWrite(SS, HIGH);

  Serial.print(buff);
  Serial.println ("Subtracting results:");
  Serial.println (a, DEC);
  Serial.println (b, DEC);
  Serial.println (c, DEC);
  Serial.println (d, DEC);
  
  delay (slaveProcessingTime);  
} 

