#ifndef BOARD_H
#define BOARD_H

#include <Arduino.h>
#include "sensor.h"
#include "q.h"

#define NB_LOOP_ITERATIONS    (2)
#define OUTPUT_BURST_LENGTH   (Q_LENGTH)

typedef long boardID; // GUID

class Board{
  protected:
    const int nbLoopIterations, 
              nbDataGets;       
    
    const byte nbSensors;
    const boardID guid;

    char incomingChar;
    boolean incomingFlag = false;
    
    Sensor **sensorVec;
    Q<timeValStrut_t> *q;

    void updateSensorData();
    void getData();
    boardID getGUID() const;

    void sendSPI(boardID id);
    void sendSPI(timeValStrut_t &tvs);
    void processCommand();
    

  public:
    Board(boardID iid, int nbSen, Sensor **sVec);

    void loop();  
    void serialEvent();
};

#endif
