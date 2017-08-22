#include "defs.h"

void show(const testS &ts){
  Serial.print("bid: ");
  Serial.println(ts.bid);
  Serial.print("cid: ");
  Serial.println(ts.cid);
  Serial.print("t  : ");
  Serial.println(ts.t);
  Serial.print("v  : ");
  Serial.println(ts.v);
}



