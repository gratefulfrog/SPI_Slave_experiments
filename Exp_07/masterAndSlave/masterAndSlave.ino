#include "app.h"

/*
 * This version sends lowercase characters and receives an arbitrary struct in response!.
 * The master sends a single lower case lettter,
 * The slave receives the lower case letter and fills a dataStruc with stuff that depends on the character received.
 * The Slave then sends the bytes that make up the dataStruct back to the master
 * The master reads the bytes into a dataStruct, and sends the next character.
 * It is interesting to monitor both Master and Slave in a ternminal.
 * 2017 08 04 works!!!
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
boolean show = true;
void loop() {
  if (show){
    Serial.print("sizeof int: ");
    Serial.println(sizeof(int));
    Serial.print("sizeof DataStruct: ");
    Serial.println(sizeof(struct DataStruct));
    show = false;
    delay(2000);
  }
  app->loop();
}
