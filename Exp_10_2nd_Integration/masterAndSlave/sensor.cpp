#include "sensor.h"


Sensor::Sensor(sensorID_t iid): sid(iid){
}

Sensor::Sensor(){}

void Sensor::getValue(timeValStruct_t &tvs) const{
  tvs.id = sid;
  tvs.v = readSensorValue();
}

void Sensor::serialPrintTVS(timeValStruct_t &tvs){
  Serial.print("sensorID_t : ");
  Serial.println(tvs.id);
  Serial.print("timeStamp_t : " );
  Serial.println(tvs.t);
  Serial.print("sensorValue_t: ");
  Serial.println(tvs.v);
}

VibrationSensor::VibrationSensor(sensorID_t iid): Sensor(iid){}
VibrationSensor::VibrationSensor():Sensor(){}

// for tests
sensorValue_t VibrationSensor::readSensorValue(){
  
  static sensorValue_t val        = 0;
  static const sensorValue_t inc  = 1;
  sensorValue_t     res           = val;
 
  val += inc;
  
  return res;
}

LightSensor::LightSensor(sensorID_t iid): Sensor(iid){}
LightSensor::LightSensor():Sensor(){}

// for tests
sensorValue_t LightSensor::readSensorValue(){
  
  static sensorValue_t val        = 0;
  static const sensorValue_t inc  = 10;
  sensorValue_t     res           = val;
  
  val += inc;
    
  return res;
}

