#ifndef ELEVATOR_DATA_H
#define ELEVATOR_DATA_H

#include "Observation.h"
#include <Arduino.h>

typedef unsigned int uint;

#define DOOR_SLIDE_TIME 2.0
#define DOOR_STOP_TIME  1.5
#define FLOOR_LENGTH    4 //Ako mijenjas ovdje, mora� i u LedControl.h
#define SPACE_LENGTH    2
#define NUMBER_OF_LEDS  28
#define MAX_VEL         2.0
#define ACC             2.0
#define GROUND_FLOOR    0
#define OPEN_STAGE      1
#define CLOSE_STAGE     -1
#define STOP            0
#define MAX_TIME        100000.0

class ElevatorData : public Observable {
public:
    ElevatorData();
    void setTargetFloor(uint targetFloor);
    bool isIdle();
    double getDistanceToFloor(uint nextFloor);
    double getTimeToTargetFloor(uint targetFloor);
    double getTimeToNextFloor(uint firstFloor, uint secondFloor);
    bool canStopAt(uint nextFloor); //razmisli da napravi� kroz passedPoint()

protected:
    double currentPosition; //[0,NUMBER_OF_LEDS-FLOOR_LENGTH]
    double currVeloc; //[-MAX_VEL,MAX_VEL]
    int doorStage;          //{OPEN_STAGE,CLOSE_STAGE,STOP}
    double getDoorPosition();
    void updateCurrentState(double interval);

private:
    double doorPosition;    //[0,1]
    double doorStall;       //[0,1]
    uint numberOfFloors;
    uint targetFloor;       //[0,numberOfFloors-1]?
    bool idleState;

    bool isBreakRegion();
    bool isAccelerateRegion();
    bool isElevatorAbove();
    bool isElevatorBelow();
    bool correctOrientation(uint nextFloor);
    bool passedBreakingPoint(uint nextFloor);
    bool isFullStop(double interval);
    bool isElevatorAtTargetFloor();
    bool canNotReachMaxVelocity(uint targetFloor);
    void moveDoor(double interval);
    double getFloorPosition(uint nextFloor);
    double getFloorPosition();
    double absVelocity();
    double getDistanceToFloor();
    double getAccDistance();
    double getDistanceBetweenFloors(int firstFloor, int secondFloor);
    double alreadyMaxTime();
    double getPartTime(uint targetFloor);
    double getFullTime(uint targetFloor);
};
#endif
