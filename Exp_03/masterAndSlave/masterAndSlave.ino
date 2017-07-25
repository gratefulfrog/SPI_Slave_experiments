
#
#include "app.h"

App *app;

// test to see if we have an ATmega328p or an ATmega2560 MCU
#if ( SIGNATURE_1 == 0x98 && SIGNATURE_2 == 0x01)
  ///// ATMEGA 2560 VALUES ////////////
  // this is the slave!
  const boolean isMaster = false;

  // SPI interrupt routine
  ISR (SPI_STC_vect){
    app->SPI_ISR ();
  }
#else
  // ATMEGA 328p
  const boolean isMaster = true;
#endif


void setup() {
  Serial.begin(115200);
  while(!Serial);
  if (isMaster){
    app = new MasterApp();
  }
  else{
    app = new SlaveApp();
  }
}

void loop() {
  app->loop();
}
