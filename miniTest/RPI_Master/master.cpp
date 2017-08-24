#include "utilities.h"
#include "spi.h"

/* A Simple Master that demonstrates SPI timing
 *  USAGE:
 *  1. Set the #define SLOW_CLOCK if the slave is running at SLOW CLOCK!
 *  2. compile the code: $ make all
 *  3. execute $ ./master
 *  4. RPI wiring:
 *     CLK   = 23 (GPIO 11) WHITE
 *     MIS0  = 21 (GPIO 09) BLUE
 *     MOSI  = 19 (GPIO 10) ORANGE
 *     SS    = 24 (GPIO 08) YELLOW
 *     GND   = 06
 *  5. execution:
 *     first connect the wires and GND to slave
 *     power up the slave, wait for it to be ready (5 led flashes)
 *     run the master
 *     the master will poll the slave and receive a 4 byte unsigned long int,
 *     if the numbers received are in sequence, all is good and every 2000 receptions, the incoming value is displayed
 *     if not, an error message is displayed an processing halts
 */

using namespace std;

#define SLOW_CLOCK
#define AEM_BOARD

#ifdef SLOW_CLOCK
#ifdef AEM_BOARD
const int timeout = 1000, // 300 us
  loopPause = 30; // 3 ms
#else
const int timeout = 300, // 300 us
  loopPause = 3; // 3 ms
#endif
#else
const int timeout = 100, //100, //us
  loopPause =  1 ;//1; //ms
#endif

uint32_t showFrequency = 2000;

const int spiSpeed = 1000000; //2000000;

SPI *spi;

void oneShot(bool once){
  uint32_t valIn = 0;
  uint8_t *p = (uint8_t*)&valIn,
          in;
  static uint32_t lastValin = 0;
  
  if (once){
    spi->transfer ('a');  
    delayMicroseconds(timeout);
  }
  
  for (int i=0;i<3;i++){
    in = spi->transfer ('#');  
    delayMicroseconds(timeout);
    p[i] =  in;
  }

  p[3] = spi->transfer ('a');  
  delayMicroseconds(timeout);
  if(!(valIn % showFrequency)){
    cout << "valIn: " << (long) valIn << endl;
    }
  if (lastValin && valIn != lastValin+1){
    cout << "Error!!! out of sequence" << endl;
    for (int i=0;i<4;i++){
      p = (uint8_t*)&valIn,
	cout << "p[" << i << "] : " << (int)p[i] << endl;
    }
    cout << "last val in : " << (long)lastValin << endl;
    cout << "val in : " << (long) valIn << endl;
    while(1);
  }
  lastValin = valIn;
}

void loop(){
  static bool once = true;
  oneShot(once);
  if (once){
    once = false;
  }
  
  delay(loopPause);
}

int main(int argc, char** argv){
  spi = new SPI(0,spiSpeed);
  cout << "Master" << endl;
  
  if (argc==1){
    const int startPause = 5;
    for (int i=0;i<startPause;i++){
      cout << "Starting in " << startPause -i << " secs..." << endl;
      delay(1000);
    }
    cout << "Go!" << endl << endl;
    
    while (true){
      loop();
    }
  }
  else{
    showFrequency=1;
    loop();
  }
  return 0;
}
