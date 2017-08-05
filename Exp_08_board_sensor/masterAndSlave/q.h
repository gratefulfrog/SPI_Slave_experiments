#ifndef Q_H
#define Q_H

#include <Arduino.h>

#define Q_LENGTH (580)

template <class T> class Q{
  protected:
    unsigned int pptr   = 0,
                 gptr   = 0,
                 qNbObj = 0;
    static const unsigned int qLen   = Q_LENGTH;
    T *q[qLen];
    
  public:
    Q(){
      for (unsigned int i = 0;  i < qLen;i++){
        q[i] = NULL; //new T();
      }
    }
    
    void push(T *elt){
      if (qNbObj == qLen){
        Serial.println("************************** Q Full! no push! *******************************");
        delete elt;
        return;
        //qNbObj--;  /// this does not work as the pointers get all screwed up, I think...
      }
      q[pptr] = elt;
      pptr = (pptr+1) % qLen;
      qNbObj++;
    }
    
    
    T* pop(){
      T* res = NULL;
      if (qNbObj){
          res = q[gptr];
          gptr = (gptr+1) % qLen;
          qNbObj -=1;
      }
      return res;
    }
};

#endif
