#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

#include "timeStamper.h"

#define NULL_SENSOR_ID (255)

typedef long sensorValue_t;
typedef byte sensorID_t;

typedef struct timeValStruct_t {
  sensorID_t id;
  timeStamp_t   t;
  sensorValue_t v;
};
  
class Sensor {
  protected:
    sensorID_t sid;
    //TimeStamper    *ts =  NULL;
    virtual sensorValue_t readSensorValue() = 0;

  public:
    Sensor(sensorID_t iid); //, TimeStamper *t);
    Sensor();
    void getValue(timeValStruct_t &tvs) const;

    static void serialPrintTVS(timeValStruct_t &tvs);

    static const sensorID_t nullSensorID = NULL_SENSOR_ID;
};

class VibrationSensor: public Sensor{
  protected:
    virtual sensorValue_t readSensorValue();
    
  public:
    VibrationSensor(sensorID_t iid);//, TimeStamper *t);
    VibrationSensor();
};

class LightSensor: public Sensor{
  protected:
    virtual sensorValue_t readSensorValue();
     
  public:
    LightSensor(sensorID_t iid); //, TimeStamper *t);
    LightSensor();
};



#endif
