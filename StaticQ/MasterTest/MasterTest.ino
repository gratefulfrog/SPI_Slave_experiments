
#include <Arduino.h>
#include <SPI.h>
#include "defs.h"
#include "SPI_anything.h"

void setup(){
  Serial.begin(115200);
  //while(!Serial);
  Serial.println("Master");
  SPI.begin();
  //delayMicroseconds(pauseBetweenSends);
  
  SPI.beginTransaction (SPISettings (1000000, MSBFIRST, SPI_MODE0));  // 2 MHz clock
  digitalWrite (SS, LOW);        // assert Slave Select
  //delayMicroseconds(pauseBetweenSends);
  
  SPI.transfer ('b');  // do a transfer
  //delayMicroseconds(pauseBetweenSends);

  byte b[4];  
  unsigned long  big;
  for (int i=0;i<400;i++){
    b[i] = SPI.transfer ('#');
    delayMicroseconds(30);
    Serial.println(b[i]);
  }
  digitalWrite (SS, HIGH);       // de-assert Slave Select
  SPI.endTransaction ();   // allow external interrupts to fire now
  for (int i=0;i<4;i++){
    Serial.println(b[i]);
  }
}

void loop(){/*  testS ts;
  SPI.beginTransaction (SPISettings (1000000, MSBFIRST, SPI_MODE0));  // 2 MHz clock
  delayMicroseconds(pauseBetweenSends);
  digitalWrite (SS, LOW);        // assert Slave Select
  delayMicroseconds(pauseBetweenSends);
  

  SPI_readAnything_reprime(b,'b');
  digitalWrite (SS, HIGH);       // de-assert Slave Select
  delayMicroseconds(pauseBetweenSends);
  SPI.endTransaction ();   
  delayMicroseconds(pauseBetweenSends);
  //show(ts);
  
  Serial.println(b);
  //delay(1000);
  */
}

