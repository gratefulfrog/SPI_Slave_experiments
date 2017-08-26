
#define Q_DEBUG
#include "staticQ.h"


typedef uint32_t qTest_t;

const qTest_t nV = -1; 

int main(){
  // creating q of 3 int elts

  Q<qTest_t> *q = new Q<qTest_t>(nV,3);

  cout << "nb elts: " << (int)q->qNbElts() << endl;
  
  for (qTest_t i=0;i< 5;i++){
    q->push(i);
  }
  cout << "nb elts: " << (int)q->qNbElts() << endl;
  qTest_t p =  q->pop();
  do{
    cout << "popped: " << (int) p << endl;
    p = q->pop();
  } while(p != nV);
  return 0;
}

