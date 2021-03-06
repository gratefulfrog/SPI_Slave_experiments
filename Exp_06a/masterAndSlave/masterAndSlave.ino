#include "app.h"

/*
 * This version sends lowercase characters and receives a struct in response???? .
 * The master sends a single lower case lettter,
 * The slave sends upper case version of the char until it gets a new char, i.e. new command.
 * The master reads 3 chars into a dataStruct
 * It is interesting to monitor both Master and Slave in a ternminal.
 * 2017 08 03 works!!!
 */

App *app;

// test to see if we have an ATmega328p or an ATmega2560 MCU
#if ( SIGNATURE_1 == 0x98 && SIGNATURE_2 == 0x01)
  ///// we have an ATMEGA 2560: Slave
  
  const boolean isMaster = false;
  // SPI interrupt routine
  ISR (SPI_STC_vect){
    app->SPI_ISR ();
  }
#else
  // we have an ATMEGA 328p: Master
  const boolean isMaster = true;
#endif

void setup() {
    app = isMaster ? static_cast<App*>(new MasterApp()) 
                   : static_cast<App*>(new SlaveApp());
}

void loop() {
  app->loop();
}
