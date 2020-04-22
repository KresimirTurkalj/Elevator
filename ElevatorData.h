#ifndef ELEVATOR_DATA_H
#define ELEVATOR_DATA_H

#define MAX_FLOOR 8

typedef unsigned int uint;

struct ElevatorParams{
    uint floorLength;
    uint spaceLength;
    uint numberOfFloors;
    double maximumVelocity;
    double acceleration;
};

class ElevatorData {
  public:
    ElevatorData(ElevatorParams elevatorParams);
    void setTargetFloor(uint targetFloor);
    uint getNumberOfRequiredLeds();

  protected:
    void updateCurrentState(double interval);
    double currentPosition;
    uint floorLength;
    int doorState;
  
  private:
    uint spaceLength;
    double currentVelocity;
    double acceleration;
    int doorCheck;
    double maximumVelocity;
    uint numberOfFloors;
    uint targetFloor;
    uint numberOfLeds;

    ElevatorData();
    void setNumberOfLeds();
    bool isBreakRegion();
    bool isAccelerateRegion();
    bool isElevatorAbove();
    bool isElevatorBelow();
    bool isFullStop(double interval);
    bool isElevatorAtTargetFloor();
    void openAndCloseDoor();
    uint getFloorPosition();
    double getDistance();
};
#endif
