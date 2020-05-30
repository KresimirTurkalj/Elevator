#include "ElevatorData.h"
#include <Arduino.h>

ElevatorData::ElevatorData(): Observable(){
  currentPosition = 0.0;
  currentVelocity = 0.0;
  targetFloor = GROUND_FLOOR;
  setMaxNumberOfFloors();
  idleState = true;
  doorPosition = 0.0;
  doorStage = OPEN_STAGE;
}

void ElevatorData::setTargetFloor(uint nextFloor){
  if(targetFloor >= 0 && targetFloor < numberOfFloors) targetFloor = nextFloor;
  idleState = false;
}

bool ElevatorData::isIdle(){
  return idleState;
}

double ElevatorData::getDoorPosition(){
  return doorPosition;
}

void ElevatorData::updateCurrentState(double interval){
  if(!isIdle()){
    if(isElevatorAtTargetFloor()){
      moveDoor(interval);
    }
    else if(isFullStop(interval)){
      currentVelocity = 0;
      currentPosition = getFloorPosition();
    }
    else if(isBreakRegion()){
      currentVelocity -= (currentVelocity > 0) ? interval*ACCELERATION : -interval*ACCELERATION;
    }
    else if(isAccelerateRegion()){
      currentVelocity += (isElevatorBelow()) ? interval*ACCELERATION : -interval*ACCELERATION;
    }
    currentPosition += currentVelocity * interval;
  }
}

bool ElevatorData::isBreakRegion(){
  if(!(isElevatorBelow() != !(currentVelocity > 0.0))){ //XOR -> !A != !B
     return true;
  }
  return (2*getDistance() <= currentVelocity*currentVelocity/ACCELERATION);
}

bool ElevatorData::isAccelerateRegion(){
  return (absVelocity() < MAX_VELOCITY && currentPosition != getFloorPosition());
}

double ElevatorData::absVelocity(){
  return (currentVelocity > 0.0) ? currentVelocity : -currentVelocity;
}

bool ElevatorData::isElevatorAbove(){
  return getFloorPosition() < currentPosition;
}

bool ElevatorData::isElevatorBelow(){
  return getFloorPosition() > currentPosition;
}

bool ElevatorData::isFullStop(double interval){ //Razmisli o Epsilon
   if(isElevatorBelow()){
     return currentPosition + currentVelocity * interval >= getFloorPosition();
    }
    else{
     return currentPosition + currentVelocity * interval <= getFloorPosition();
    }
}

bool ElevatorData::isElevatorAtTargetFloor(){
  return (!isElevatorAbove() && !isElevatorBelow() && currentVelocity == 0);
}

void ElevatorData::moveDoor(double interval){ //razmisli da izvedeš s intervala, zasad prolazi.
  if(doorStage != STOP){
    doorPosition += (double) ( interval / DOOR_SLIDE_TIME * doorStage);
    if(doorPosition >= 1){
      doorStage = STOP;
    }
    else if(doorPosition <= 0){
      doorStage = OPEN_STAGE;
      idleState = true;
      taskEnded();
      DEBUG = true;
    }
  }
  else if(doorStall < 1){
    doorStall += interval / DOOR_STOP_TIME;
  }
  else{
    doorStage = CLOSE_STAGE;
  }
}

double ElevatorData::getDistance(){
  return (currentPosition > getFloorPosition()) ? (currentPosition - getFloorPosition()) : (getFloorPosition() - currentPosition);
}

uint ElevatorData::getFloorPosition(){
  return (FLOOR_LENGTH+SPACE_LENGTH)*targetFloor;
}

void ElevatorData::setMaxNumberOfFloors(){
  numberOfFloors = (NUMBER_OF_LEDS - FLOOR_LENGTH) / (FLOOR_LENGTH + SPACE_LENGTH);
}
