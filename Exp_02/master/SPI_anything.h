#ifndef SPI_anything_h
#define SPI_anything_h

#include <Arduino.h>

const int pauseBetweenSends   = 10;  // microseconds

template <typename T> unsigned int SPI_writeAnything (const T& value)
  {
    const byte * p = (const byte*) &value;
    unsigned int i;
    for (i = 0; i < sizeof value; i++){
          SPI.transfer(*p++);
          delay(pauseBetweenSends);
    }
    return i;
  }  // end of SPI_writeAnything

template <typename T> unsigned int SPI_readAnything(T& value)
  {
    byte * p = (byte*) &value;
    unsigned int i;
    for (i = 0; i < sizeof value; i++)
          *p++ = SPI.transfer (0);
    return i;
  }  // end of SPI_readAnything
  
  
template <typename T> unsigned int SPI_readAnything_ISR(T& value)
  {
    byte * p = (byte*) &value;
    unsigned int i;
    *p++ = SPDR;  // get first byte
    for (i = 1; i < sizeof value; i++)
          *p++ = SPI.transfer (0);
    return i;
  }  // end of SPI_readAnything_ISR  


#endif
