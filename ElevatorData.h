#ifndef ELEVATOR_DATA_H
#define ELEVATOR_DATA_H

#include "Observation.h"

typedef unsigned int uint;

#define DOOR_SLIDE_TIME 3.0
#define DOOR_STOP_TIME  3.0
#define FLOOR_LENGTH    4 //Ako mijenjaš ovdje, moraš i u LedControl.h
#define SPACE_LENGTH    2
#define NUMBER_OF_LEDS  39
#define MAX_VELOCITY    2.0
#define ACCELERATION    2.0
#define GROUND_FLOOR    0
#define OPEN_STAGE      1
#define CLOSE_STAGE     -1
#define STOP            0

class ElevatorData: public Observable{
  public:
    ElevatorData();
    void setTargetFloor(uint targetFloor);
    bool isIdle();

  protected:
    double currentPosition; //[0,NUMBER_OF_LEDS-FLOOR_LENGTH]
    int doorStage;          //{OPEN_STAGE,CLOSE_STAGE,STOP}
    double getDoorPosition();
    void updateCurrentState(double interval);
  
  private:
    double currentVelocity; //[-MAX_VEL,MAX_VEL]
    double doorPosition;    //[0,1]
    double doorStall;       //[0,1]
    uint numberOfFloors;    
    uint targetFloor;       //[0,numberOfFloors-1]?
    bool idleState;        
    
    bool isBreakRegion();
    bool isAccelerateRegion();
    bool isElevatorAbove();
    bool isElevatorBelow();
    bool isFullStop(double interval);
    bool isElevatorAtTargetFloor();
    void moveDoor(double interval);
    uint getFloorPosition();
    double absVelocity();
    double getDistance();
    void setMaxNumberOfFloors();
};
#endif
