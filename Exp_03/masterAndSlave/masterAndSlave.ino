#include "app.h"

/*
 * This version sends structs to the slave.
 * Both master and slave output to Serial port so 2 terminals are necessary to monitor processing!
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
