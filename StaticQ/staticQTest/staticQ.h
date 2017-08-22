#ifndef Q_H
#define Q_H

#include <Arduino.h>

#define Q_Q_LENGTH              (10)
#define Q_MAX_SIZE              (256)
#define Q_OVERRUN_DELETE_OLDEST (1)

/** Constructor of a template Queue class
 * Usage:
 * type T should be a copiable object you want to enqueue. The object will be copied into the q, physically
 * 
 * calls to all methods of this class should be enclosed in noInterrupts()...interrupts() to prevent bad things from
 * happening by re-etnrant code!
 * In case of over-run,  depending  on the value of Q_OVERRUN_DELETE_OLDEST, either the oldest elt will be overwritten
 *    or the new elt will not be pushed onto the queue.
 ** at consturction a nullObject must be provided to be used in pop if q is empty
 ** push returns void, but will display overun LED if q is full
 ** pop will return a copy of the object or the null object if q is empt
 */
template <class T> 
class Q{
 protected:
  volatile uint8_t pptr   = 0,  /*!< volatile (for use via interrupt) index of the next spot to put an elt */
    gptr   = 0,  /*!< volatile (for use via interrupt) index of the next spot to get an elt */
    qNbObj = 0;  /*!< volatile (for use via interrupt) nb of elts in queue */
  static const uint8_t qLen   = Q_Q_LENGTH > Q_MAX_SIZE ? Q_MAX_SIZE : Q_Q_LENGTH; /*!< max nb of elts that the queue can contain */
  static const bool overrunDeleteOldest = (bool) Q_OVERRUN_DELETE_OLDEST; /*!< if true when queue is full, delete oldest elt, otherwise do not enqueue new elts  */
  const T nullElt;
  const int ledPin;
  T q[qLen];

  void incPtr(volatile uint8_t &p){
    p = (p+1) % qLen;
  }

  void setFullLed(bool onOff){
    digitalWrite(ledPin, onOff);
  }
  
 public:
  /*!< constructer, inits the nullElt */
  Q(T nE, int ld) : nullElt(nE), ledPin(ld){
      pinMode(ledPin,OUTPUT);
      setFullLed(false);
  } 
    /** copy anew elt onto the queue
     *  @param *elt pointer to the let to be enqueued */
  void push(T elt){
    if (qNbObj == qLen){          // the q is full, inform by  LED, and  apply deletion strategy
      setFullLed(true);
      if (overrunDeleteOldest){   // in this case delete the oldest and continue with the enqueing of the current elt.
      	incPtr(gptr);	
      	qNbObj--;
      }
      else{ // we do not push, in this case do nothing
        return;
      }
    }
    // at this point: either the Q is not full, or the Q was full and the oldest element has been deleted to make room for one more!
    q[pptr] = elt;
    incPtr(pptr);
    qNbObj++;
  }
    
    /** pops the oldest elt off the queue
     *  @return the elt popped, or nullElt if queue is empty.
     */
  T pop(){
    setFullLed(false);
    T res =  nullElt;
    if (qNbObj){
      res = q[gptr];
      incPtr(gptr);
      qNbObj--;
    }
    return res;
  }
  unsigned int qNbObjects() const{ /*!< return nb of objects in queue */
    return qNbObj;
  }
  void clear(){
    pptr = gptr = qNbObj = 0;  /*!< resets q to initial state */
  }
};

#endif
  
