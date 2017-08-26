#ifndef Q_H
#define Q_H

//#define Q_DEBUG
//#define Q_ARUDINO

#define Q_Q_LENGTH              (3)
#define Q_MAX_SIZE              (256)
#define Q_OVERRUN_DELETE_OLDEST (0)


void outMessage(char*);

#ifdef Q_ARUDINO
#include <Arduino.h>

#ifdef  Q_DEBUG
void outMessage(const char *msg){
  Serial.print(msg);
}
#else
void outMessage(const char *msg({}
#endif

#else 
#include <iostream>
#include <stdint.h>
#include <stdio.h>

using namespace std;

#ifdef  Q_DEBUG
void outMessage(const char *msg){
  cout << msg;
}
#else
void outMessage(const char *msg({}
#endif

#endif



/** define the units for q dimensions and size */
typedef uint8_t qDim_t;

/** Constructor of a template Queue class
 * Usage:
 * type T should be any copyable object you want to enqueue, a copy will be enqueued.
 * 
 * calls to all methods of this class may need to be enclosed in noInterrupts()...interrupts() to prevent bad things from
 * happening by re-etnrant code!
 * In case of over-run, the pointer will be used to delete the most recent or last object object, depending 
 * on the value of Q_OVERRUN_DELETE_OLDEST
 */
template <class T> class Q{
protected:
  volatile qDim_t pptr   = 0,  /*!< volatile (for use via interrupt) index of the next spot to put an elt */
    gptr   = 0,  /*!< volatile (for use via interrupt) index of the next spot to get an elt */
    nbElts = 0;  /*!< volatile (for use via interrupt) nb of elts in queue */
  const qDim_t qLength   = Q_Q_LENGTH > Q_MAX_SIZE ? Q_MAX_SIZE : Q_Q_LENGTH; /*!< max nb of elts that the queue can contain */
  const bool overrunDeleteOldest = (bool) Q_OVERRUN_DELETE_OLDEST; /*!< if true when queue is full, delete oldest elt, otherwise do not enqueue new elts  */
  
  const int ledPin;
  
  T *q;
  
  void inc(volatile qDim_t &targ, volatile qDim_t lim){
    targ = (targ+1) % lim;
  }
  
  void setFullLed(bool onOff){
#ifdef Q_ARUDINO
    digitalWrite(ledPin, onOff);
#else
    outMessage("Q Full: ");
    outMessage((char*) onOff ? "True" : "False");
#endif
  }
  void nullifyAll() {
    for (qDim_t i = 0;  i < qLength;i++){
      q[i] = nullVal;
    }
  }
  
public:
  const T nullVal;
  
  /** constructor, sets all pointers to NULL */
  Q(T nulV, int led): nullVal(nulV), ledPin(led) {
#ifdef Q_ARUDINO
    pinmode(ledPin,OUTPUT);
#endif
    q = new T[qLength];
    nullifyAll();
  }
  
  /** push a pointer to a new elt onto the queue
   *  @param *elt pointer to the let to be enqueued */
  void push(T &elt){
    if(nbElts == qLength){
      // the q is full, now apply deletion strategy
      if (overrunDeleteOldest){
	// in this case pop the oldest and continue with the enqueing of the current elt.
	outMessage("** Q Full! deleting oldest! **\n");
	pop();
      }
      else{
	// we do not push
	outMessage("** Q Full! no push! **\n");
	return;
      }
    }
    q[pptr] = elt;
    inc(pptr,qLength);
    nbElts++;
  }
  
  /** pops the oldest elt off the queue
   *  @return a pointer to the elt popped, or NULL if queue is empty. the elt is not deleted from heap.
   */
  T pop(){
    T res = nullVal;
    if (nbElts > 0){
      res = q[gptr];
      inc(gptr,qLength);
      nbElts--;
    }
    return res;
  }
  
  qDim_t qNbElts() const{
    return nbElts;
  }
  
  void clear(){                    /*!< resets q to initial state */
    pptr = gptr = qNbElts = 0;
    nullifyAll();
    setFullLed(false);
  }
};

#endif

