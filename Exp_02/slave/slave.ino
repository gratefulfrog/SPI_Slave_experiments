// slave

#include <SPI.h>
#include "SPI_anything.h"

// create a structure to store the different data values:
typedef struct myStruct{
  byte a;
  int b;
  long c;
};

volatile myStruct foo;
volatile bool haveData = false;

void setup (){
  Serial.begin (115200);   // debugging

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // now turn on interrupts
  SPI.attachInterrupt();  
}

void loop () { 
  if (haveData){
     Serial.println ((int) foo.a);
     Serial.println (foo.b);
     Serial.println (foo.c);
     Serial.println ();
     haveData = false;
  }
}  

// SPI interrupt routine
ISR (SPI_STC_vect){
  SPI_readAnything_ISR (foo);
  haveData = true;
}
