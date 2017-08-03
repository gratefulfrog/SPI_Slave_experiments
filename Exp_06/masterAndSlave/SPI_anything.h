#ifndef SPI_anything_h
#define SPI_anything_h

#include <Arduino.h>
//#include "app.h"

const char nullChar = '#';
const int pauseBetweenSends = 20; //us

template <typename T> unsigned int SPI_writeAnything (const T& value){
    const byte * p = (const byte*) &value;
    unsigned int i=0,
                lim = sizeof value;
    for (; i < lim; i++){
          SPI.transfer(*p++);
          delay(pauseBetweenSends);
    }
    return i;
  }  // end of SPI_writeAnything

template <typename T> unsigned int SPI_readAnything(T& value){
  byte * p = (byte*) &value;
  unsigned int i = 0,
               lim = sizeof value;
  for (; i < lim; i++)
        *p++ = SPI.transfer (nullChar);
  return i;
}
 
template <typename T> unsigned int SPI_readAnything_reprime(T& value, byte prime){
  byte * p = (byte*) &value;
  unsigned int i =0,
               limLessOne = (sizeof value) -1;
  for (; i < limLessOne; i++){
    byte got = SPI.transfer (prime);
    Serial.print("got: ");
    Serial.println(char(got));  
    *p++ = got;
  }
  *p++ = SPI.transfer (prime);
  return i;
}
 
template <typename T> unsigned int SPI_readAnything_ISR(T& value){
  byte * p = (byte*) &value;
  unsigned int i=1,
               lim = sizeof value;
  *p++ = SPDR;  // get first byte
  for (; i < lim; i++)
        *p++ = SPI.transfer (nullChar);
  return i;
} 

#endif
