
#include <Arduino.h>
#include <SPI.h>

/* A Simple Master that demonstrates SPI timing
 *  USAGE:
 *  1. Set the #define SLOW_CLOCK if the slave is running at SLOW CLOCK!
 *  2. upload the code.
 *  3. activate a serial monitor
 *  4. UNO wiring:
 *     CLK   = 13 WHITE
 *     MIS0  = 12 BLUE
 *     MOSI  = 11 ORANGE
 *     SS    = 10 YELLOW
 *  5. execution:
 *     first connect the wires and GND to slave
 *     power up the slave, wait for it to be ready (5 led flashes)
 *     activate the serial monitor
 *     reset the master
 *     the master will poll the slave and receive a 4 byte unsigned long int,
 *     if the numbers received are in sequence, all is good and every 2000 receptions, the incoming value is displayed
 *     if not, an error message is displayed an processing halts
 */


#define SLOW_CLOCK


#ifdef SLOW_CLOCK
const int timeout = 200, //us
  loopPause = 2; //ms
#else
const int timeout = 100, //us
  loopPause = 1; //ms
#endif

const uint32_t showFrequency = 2000;

uint8_t b[4];

SPISettings settings;

void setup(){
  Serial.begin(115200);
  SPI.begin();
  Serial.println("Master");  

  settings = SPISettings (2000000, MSBFIRST, SPI_MODE0);
}

void oneShot(bool once){
  uint32_t valIn = 0;
  uint8_t *p = (uint8_t*)&valIn,
          in;
  static uint32_t lastValin = 0;
  
  if (once){
    SPI.beginTransaction (settings);
    digitalWrite (SS, LOW); 
    SPI.transfer ('a');  
    delayMicroseconds(timeout);
    digitalWrite (SS, HIGH);
    SPI.endTransaction ();  
  }
  
  for (int i=0;i<3;i++){
    SPI.beginTransaction (settings);
    digitalWrite (SS, LOW);
    //delayMicroseconds(timeout);
    in = SPI.transfer ('#');  
    delayMicroseconds(timeout);
    p[i] =  in;
    digitalWrite (SS, HIGH);
    SPI.endTransaction (); 
  }

  SPI.beginTransaction (settings);
  digitalWrite (SS, LOW); 
  //delayMicroseconds(timeout);
  p[3] = SPI.transfer ('a');  
  delayMicroseconds(timeout);
  digitalWrite (SS, HIGH);
  SPI.endTransaction (); 
  if(!(valIn % showFrequency)){
    Serial.print("valIn: ");
    Serial.println(valIn);
  }
  if (lastValin && valIn != lastValin+1){
    Serial.println("Error!!! out of sequence");
    for (int i=0;i<4;i++){
      p = (uint8_t*)&valIn,
      Serial.println(String("p[") + String(i) + String("] : ") + String(p[i]));
    }
    Serial.println(String("last val in : ") + String(lastValin));
    Serial.println(String("val in : ") + String(valIn));
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
