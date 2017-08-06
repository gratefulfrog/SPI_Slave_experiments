#include "app.h"

/*
 * This version is slave only and MEGA only.
 * It simulates a board with 2 simulated sensors.
 * The data is read into a q.
 * Serial input an 'a' to get the data out.
 * Serial input an 'i' to get the board's GUID
 * if the q is not unloaded before it is full, it will block until there is space available in the q.
 * 580 elts seems to be the largest Q length possible before crashing...
 */

App *app;

long counter = 0;

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
  if (isMaster || !counter){
    app->loop();
  }
  counter = (counter + 1) % SLAVE_LOOP_ITERATIONS;
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


