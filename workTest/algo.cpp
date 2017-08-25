#include <iostream>
#include <stdint.h>
#include <stdio.h>

using namespace std;

/************** MASTER **************/ 
/*
void MasterLoop(){
  gatherData();
  letSlaveWork();
}
*/

/************** SLAVE **************/ 

typedef uint16_t qDim_t;

uint8_t loopCount = 1;

const uint32_t nullVal = -1;
const qDim_t QLength =  3,
  objectSize = sizeof (uint32_t),
  objectSizeMinusOne = objectSize -1;

///// Q STUFF /////////
// Q stuff
qDim_t  gptr = 0,
  pptr = 0,
  nbElts;
uint32_t q[QLength];


void inc(qDim_t &targ, qDim_t lim){
  targ = (targ+1) % lim;
}

void push(uint32_t &elt){
  if(nbElts == QLength){
    cout << "q full no push!" << endl;
  }
  else{
    q[pptr] = elt;
    inc(pptr,QLength);
    nbElts++;
  }
}

uint32_t pop(){
  uint32_t res = nullVal;
  if (nbElts > 0){
    res = q[gptr];
    inc(gptr,QLength);
    nbElts--;
  }
  return res;
}

////end of q stuff ////

// data struct stuff

char*  showU32(uint32_t u, char *buff){
  sprintf(buff,"%u", u);
  return buff;
}

char* showU8(uint8_t u, char* buff){
  sprintf(buff,"%u", u);
  return buff;
}

void u32toByteVec(const uint32_t &inU32, uint8_t oVec[]){
  uint8_t *ptr =  (uint8_t*)&inU32;
  for (uint8_t i = 0; i< objectSize ; i++){
    oVec[i] = *(ptr+i);
  }
}
void byteVecToU32(uint32_t &outU32, uint8_t *vec){
  outU32 = *(uint32_t*)vec;
}

char* showReconstitutedU32(uint8_t vec[], char* buff){
  uint32_t res;
  byteVecToU32(res, vec);
  showU32(res, buff);
  return buff;
}


void popAndPut(uint8_t *vec){
  u32toByteVec(pop(),vec);
}

void showVec4(uint8_t *vec){
  char buff[10],
    bufff[10];
  for (uint8_t j = 0;j<objectSize;j++){
    showU8(j,buff);
    showU8(vec[j],bufff) ;
    cout << "vec[" << buff << "]: " <<  bufff << endl;
  }
}

bool isNullVec(uint8_t *vec){
  for (uint8_t i=0;i<objectSize;i++){
    if (vec[i] != 255){
      return false;
    }
  }
  return true;
}

// end data struct stuff ////

//ISR(SPI_STC_vect){
void testISR(uint8_t &SPDR, bool &startWorking, uint8_t *outVec){
  static qDim_t outIndex = 0;
  if(SPDR == 'a' || outIndex == 0){
    popAndPut(outVec);
  }
  SPDR = outVec[outIndex];
  startWorking = (outIndex == objectSizeMinusOne &&
		  isNullVec(outVec)) ;
  inc(outIndex,objectSize);
}

void simulateBeingPolled(bool &sw){
  cout << "being polled..." << endl;
  // simulate polling
  uint8_t ov[objectSize];
  uint8_t spdr = 'a';
  testISR(spdr, sw, ov);
  do{
    char buff[10];
    showU8(spdr, buff);
    cout << "SPDR: " << buff << endl;
    spdr = '#';
    testISR(spdr, sw, ov);
  } while(!sw);
  char buff[10];
  showU8(spdr, buff);
  cout << "SPDR: " << buff << endl;
}  

void noInterrupts(){
}


// this is the slave loop
int main(){
  bool startWorking = true;
  // loop count is only for simulations the real thing would only go through once
  while(loopCount--){
    if (startWorking){
      // here we would gather sensor data...
      cout << "Start working: " << (char*)(startWorking ? "true" : "false") << endl;
      // and enqueue it 
      for (uint32_t i = 0; i< QLength;i++){
	push(i);
	cout << "working..." << endl;
      }
    }
    // at this point we are waiting for the master to poll us, so we do nothing more
    // this call is only for simulation
    simulateBeingPolled(startWorking);
  }
  cout << "Start working: " << (char*)(startWorking ? "true" : "false") << endl;
  cout << "Done!" << endl;
}

/*
void SlaveLoop(){
  byte oldSREG = SREG;   // remember if interrupts are on or off
 
  noInterrupts ();   // turn interrupts off
  if (startWorking){
    endTime = millis() + workTime;  // work time contains a margin to be sure!
    startWorking = false;
  }
  SREG = oldSREG;  // turn interrupts back on, if they were on before
  
  if (millis() < endTime){
    work();
    b = &q.gptr;
  }
}
*/
