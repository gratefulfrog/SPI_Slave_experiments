#include "app.h"

/*
 * This version sends lowercase characters and receives upper case characters in response.
 * It is interesting to monitor both Master and Slave in a ternminal
 */

#define SLAVE (0)

App *app;

// test to see if we have an ATmega328p or an ATmega2560 MCU
//#if ( SIGNATURE_1 == 0x98 && SIGNATURE_2 == 0x01)
#if ( SLAVE )
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
