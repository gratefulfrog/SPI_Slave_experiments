#include "app.h"

/*
 * This version is Master polls slave with commands 't' 'i' or 'a' via SPI: 
 **  t: sets slave time to zero, clears q, then ready to go!
 **  i: identify expets a GUID in repsonse 
 **  a: acquire expects a timeValStruct_t in respnse
 * Slave repsonds apprpriately.
 * Slave will crash if Q over flows!
 * Slave needs to pause between reading sensors or the q will over flow since the Master cannot get the replies fast enough...
 * if no slave data is available at poll, the nullReturn is sent.
 * 
 * Tuniing the Timing : This is needed to avoid board Q overflow and catastrophic failures. It seems that the fastest we 
 *                    : do is a sensor reading every 7740 microseconds, which is not great... 
 * 
 * SPI_anything.h:  
 **  const int pauseBetweenSends : is the time in micro-seconds that is waited after an SPI.transfer or a digitalWrite(SS, LOW); 
 * app.h:
 **  App:slaveProcessingTime : is the time waited between each poll from the master to the slave, this should be zero! 
 *                           : it's ok if this is too high since a nullReturn value will be sent by the slave if no new data is available
 *                           : the nullREturn can be ignored by the master.
 * board.h:
 **  SLAVE_LOOP_ITERATIONS : the frequency of board sensor data aquisitions, 
 *                         : i.e. 1 time in SLAVE_LOOP_ITERATIONS; value 1 would be every loop, but then the q will overflow!
 *  
 */

App *app;

// test to see if we have an ATmega328p or an ATmega2560 MCU
#if ( SIGNATURE_1 == 0x98 && SIGNATURE_2 == 0x01)
  ///// we have an ATMEGA 2560: so it's the Slave
  
  const boolean isMaster = false;
  // SPI interrupt routine must be defined in the Slave, only
  ISR (SPI_STC_vect){
    app->SPI_ISR ();
  }
#else
  // we have an ATMEGA 328p: it's the Master
  const boolean isMaster = true;
#endif

void setup() {
    app = isMaster ? static_cast<App*>(new MasterApp()) 
                   : static_cast<App*>(new SlaveApp());
}

void loop() {
  app->loop();
}

void serialEvent(){
  app->serialEvent();
}

