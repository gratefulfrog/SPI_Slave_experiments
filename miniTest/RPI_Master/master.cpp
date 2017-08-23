#include "utilities.h"
#include "spi.h"

using namespace std;

const int timeout = 100, //us
  loopPause = 1; //ms
const uint32_t showFrequency = 2000;

const int spiSpeed = 2000000;

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

int main(){
  spi = new SPI(0,spiSpeed);
  cout << "Master" << endl;
  for (int i=0;i<5;i++){
    cout << "Starting in " << 5 -i << " secs..." << endl;
    delay(1000);
  }
  cout << "Go!" << endl << endl;
  
  while (true){
    loop();
  }
  return 0;
}
