#include <Arduino.h>
#include <SPI.h>

//#define SLOW_CLOCK


const int ledPin = 3;
const uint32_t showFrequency = 500;

const int incDelay = 500; //us

volatile long fired = 0;
volatile uint32_t valOut;
uint8_t *ptr;
  
volatile uint8_t  *b[4],
                  inByte,
                  outIndex;

void flashLed(uint8_t nb){
  nb*=2;
  while(nb--){
    digitalWrite(ledPin,!digitalRead(ledPin)),
    delay(500);
  }
}

void setup(){
#ifdef SLOW_CLOCK
    noInterrupts();
    CLKPR = _BV(CLKPCE);  // enable change of the clock prescaler
    CLKPR = _BV(CLKPS0);  // divide frequency by 2
    interrupts();
#endif

  valOut = 1;
  ptr = (uint8_t*)&valOut;
  outIndex = 0;
  //Serial.begin(115200);
  //Serial.println("Slave");
  pinMode(ledPin,OUTPUT);
  flashLed(5);
  
  for (int i=0;i<4;i++){
    b[i] = ptr+i;
    Serial.println(String("b[") + String(i) + String("] : ") + String(*b[i]));
  }
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  // turn on interrupts
  SPCR |= _BV(SPIE);
}

void loop(){
  static long lastFired=0,
               lastValOut = 0;
  noInterrupts();
  if (fired != lastFired){
    lastFired = fired;
    valOut = (fired % 4) ? valOut : valOut+1;
  }
  
  if(valOut  != lastValOut && !(valOut % showFrequency)){
    digitalWrite(ledPin,!digitalRead(ledPin));
    //Serial.println(valOut);
    lastValOut = valOut;
  }
  interrupts();
}

ISR(SPI_STC_vect){
  inByte = SPDR;
  outIndex = (inByte == 'a') ? 0 : min(outIndex+1,3); 
  SPDR = *b[outIndex];
  fired++;
}
