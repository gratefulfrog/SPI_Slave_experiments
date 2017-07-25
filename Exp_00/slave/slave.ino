// Written by Nick Gammon
// February 2011

// slave receives and prints what it gets to serial montior
// but sometimes misses a character!
// let's try to make a critical section using noInterrupts();
// failed!
// every 9 messages, it misses one character!
// then, by adding a pause between sends of each char, it's ok!


#include <SPI.h>

char buf [100];
volatile byte pos;
volatile boolean process_it;

void setup (void) {
  Serial.begin (115200);   // debugging
  
  // turn on SPI in slave mode
  SPCR |= bit (SPE);

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // get ready for an interrupt 
  pos = 0;   // buffer empty
  process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
byte c = SPDR;  // grab byte from SPI Data Register
  
  // add to buffer if room
  if (pos < (sizeof (buf) - 1))
    buf [pos++] = c;
    
  // example: newline means time to process buffer
  if (c == '\n')
    process_it = true;
}

// main loop - wait for flag set in interrupt routine
// note creation of critical section! 
// critical section is unneeded because the master waits to allow the slave to process!  
// and anyway it doesn't work!
void loop (void){
  //noInterrupts();
  if (process_it) {
    buf [pos] = 0;  
    Serial.print("Receive: ");  
    Serial.print (buf);
    pos = 0;
    process_it = false;
    }  // end of flag set
  //interrupts();
}
