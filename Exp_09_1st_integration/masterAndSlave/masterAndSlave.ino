#include "app.h"

/*
 * This version is Master polls slave with commands 'i' or 'a' via SPI: 
 **  i: identify expets a GUID in repsonse 
 **  a: acquire expects a timeValStrut_t in respnse
 * Slave repsonds apprpriately.
 * Slave will crash if Q over flows!
 * Slave needs to pause between reading sensors or the q will over flow since the Master cannot get the replies fast enough...
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
  // counter strategy is used to slow down slave data production and thus allow human observer some
  // time to see what's going on
  app->loop();
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  //Serial.println("serialevent");
  app->serialEvent();
}


