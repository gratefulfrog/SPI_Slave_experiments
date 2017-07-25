// Written by Nick Gammon
// April 2011

/*
 * This version requires pin2 on the slave to be conneted to the SS pin, in parallel;
 * Note: pin2 => interrupt 0, in attach interrupt
 */

const int interruptPin = 2;

// what to do with incoming data
byte command = 0;

// start of transaction, no command yet
void ss_falling (){
  command = 0;
}  

void setup (void){
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

  // interrupt for SS falling edge
  attachInterrupt (digitalPinToInterrupt(interruptPin), ss_falling, FALLING);
} 


// SPI interrupt routine
ISR (SPI_STC_vect){
  byte c = SPDR;
 
  switch (command){
  // no command? then this is the command
  case 0:
    command = c;
    SPDR = 0;
    break;
    
  // add to incoming byte, return result
  case 'a':
    SPDR = c + 15;  // add 15
    break;
    
  // subtract from incoming byte, return result
  case 's':
    SPDR = c - 8;  // subtract 8
    break;
  } 
}  

void loop (void){
  // all done with interrupts
}
