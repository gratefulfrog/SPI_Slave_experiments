// master

#include <SPI.h>
#include "SPI_anything.h"

const int pauseBetweenSends   = 10,  // microseconds
          bigBuffSize         = 100, // enough space for a long string
          slaveProcessingTime = 2000; // millisecs


// create a structure to store the different data values:
typedef struct myStruct
{
  byte a;
  int b;
  long c;
};

myStruct foo;

void setup (){
  Serial.begin (115200);
  
  SPI.begin ();
  
  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV8);

  foo.a = 42;
  foo.b = 32000;
  foo.c = 100000;
}  // end of setup

void loop () {
  char buff[bigBuffSize];
  outgointMsg(buff);

  Serial.print(buff);
  digitalWrite(SS, LOW);    // SS is pin 10
  SPI_writeAnything (foo);
  digitalWrite(SS, HIGH);
  delay (slaveProcessingTime);  // for testing  
  
  foo.c++;
}  // end of loop


void outgointMsg(char* buf){
  // puts a char[] into the arg buf,

  static int sendCount = 0;
  String s = "Send: " + String(sendCount++) + " !\n";
  s.toCharArray(buf,s.length()+1);
}

