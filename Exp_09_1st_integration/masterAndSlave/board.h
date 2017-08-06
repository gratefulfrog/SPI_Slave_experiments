#ifndef BOARD_H
#define BOARD_H

#include <Arduino.h>
#include "sensor.h"
#include "q.h"

#define OUTPUT_BURST_LENGTH   (Q_LENGTH)
// set the following value to (1) for maximum slave speed!
// SPI to UNO seems like 6 is the fastest we can go!
#define SLAVE_LOOP_ITERATIONS (6) 

typedef unsigned long boardID; // GUID
typedef unsigned long microTime;

class Board{
  protected:
    
    long counter = 0;  // for simulation
    const int nbDataGets;       
    
    const byte nbSensors;
    const boardID guid;

    char incomingChar;
    boolean incomingFlag = false;
    
    Sensor **sensorVec;
    Q<timeValStrut_t> *q;

    void updateSensorData();
    void getData();
   

    void sendSPI(boardID id);
    void sendSPI(timeValStrut_t &tvs);
    void processCommand();
    

  public:
    
    Board(boardID iid, int nbSen, Sensor **sVec);
    boardID getGUID() const;
    void loop();  
    void serialEvent();
    timeValStrut_t *pop();
    void showQSize() const;
};

#endif
