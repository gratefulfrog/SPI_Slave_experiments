#include "utilities.h"
#include "spi.h"

using namespace std;

const int timeout = 125, //us
          loopPause = 0; //ms
const uint32_t showFrequency = 2000;

uint8_t b[4];

const int spiSpeed = 2000000;

SPI *spi;

/*
void setup(){
  Serial.begin(115200);
  SPI.begin();
  Serial.println("Master");  

  settings = SPISettings (2 000 000, MSBFIRST, SPI_MODE0);
}
*/
int main(){
  spi = new SPI(0,spiSpeed);
  cout << "Master" << endl;
  while (true){
    loop();
  }
  return 0;
}

  
void oneShot(bool once){
  uint32_t valIn = 0;
  uint8_t *p = (uint8_t*)&valIn,
          in;
  static uint32_t lastValin = 0;
  
  if (once){
    //SPI.beginTransaction (settings);
    //digitalWrite (SS, LOW); 
    SPI.transfer ('a');  
    delayMicroseconds(timeout);
    //digitalWrite (SS, HIGH);
    //SPI.endTransaction ();  
  }
  
  for (int i=0;i<3;i++){
    //SPI.beginTransaction (settings);
    //digitalWrite (SS, LOW);
    //delayMicroseconds(timeout);
    in = SPI.transfer ('#');  
    delayMicroseconds(timeout);
    p[i] =  in;
    //digitalWrite (SS, HIGH);
    //SPI.endTransaction (); 
  }

  //SPI.beginTransaction (settings);
  //digitalWrite (SS, LOW); 
  //delayMicroseconds(timeout);
  p[3] = SPI.transfer ('a');  
  delayMicroseconds(timeout);
  //digitalWrite (SS, HIGH);
  //SPI.endTransaction (); 
  if(!(valIn % showFrequency)){
    cout << "valIn: " << (long) valIn << endl;
  }
  if (lastValin && valIn != lastValin+1){
    cout << "Error!!! out of sequence" << endl;
    for (int i=0;i<4;i++){
      p = (uint8_t*)&valIn,
	cout << "p[" << i << "] : " << p[i] << endl;
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
