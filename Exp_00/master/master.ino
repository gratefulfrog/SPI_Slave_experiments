// Written by Nick Gammon
// February 2011

// master spi
// sends "Hello, world: n !\n" to slave, where n is the send counter

/* connections :
 * UNO is master, MEGA is slave,
 * SS to SS
 * CLK to CLK
 * MOSI to MOSI
 * MISO to MISO (do not interchange MISO and MOSI)
 * GND, 5v
 * Some tricks:
 * a 10 us pause between char sends is needed since the MEGA can't keep up otherwise
 * on the slave side, I created a critical section, with no interrupts,but this is not needed because 
 * the master waits between each full message, allowing the slave time to process!
 * This all works ok!
 */


#include <SPI.h>

const int pauseBetweenSends   = 10,  // microseconds
          bigBuffSize         = 100, // enough space for a long string
          slaveProcessingTime = 2000; // millisecs
          
void setup (void){
  Serial.begin(115200);
  digitalWrite(SS, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();

  // Slow down the master a bit
  // not needed!
  //SPI.setClockDivider(SPI_CLOCK_DIV8); 
}

void outgointMsg(char* buf){
  // puts a char[] into the arg buf,
  // msg is "Hello, world: " plus a send counter

  static int sendCount = 0;
  String s = "Hello, world: " + String(sendCount++) + " !\n";
  s.toCharArray(buf,s.length()+1);
}

void loop (void){
  char c;
  char buff[bigBuffSize];
  outgointMsg(buff);
 
  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

  Serial.print("Send: ");
  Serial.print(buff);

  // send test string
  for (const char * p = buff; c = *p; p++){
    SPI.transfer (c);
    delayMicroseconds(pauseBetweenSends);
  }

  // disable Slave Select
  digitalWrite(SS, HIGH);

  delay (slaveProcessingTime);  // 1 seconds delay 
}  // end of loop
