#include "ElevatorData.h"

ElevatorData::ElevatorData(ElevatorParams elevatorParams): TargetObservable(){
  numberOfFloors = elevatorParams.numberOfFloors;
  floorLength = elevatorParams.floorLength;
  spaceLength = elevatorParams.spaceLength;
  numberOfLeds = elevatorParams.numberOfFloors * elevatorParams.floorLength + (elevatorParams.numberOfFloors - 1) * elevatorParams.spaceLength;
  maximumVelocity = elevatorParams.maximumVelocity;
  acceleration = elevatorParams.acceleration;
  currentPosition = 0.0;
  currentVelocity = 0.0;
  targetFloor = 1;
  doorState = 0;
  doorCheck = 0;
}

void ElevatorData::setTargetFloor(uint targetFloor){
  if(targetFloor >= 0 && targetFloor < numberOfFloors) this->targetFloor = targetFloor;
}

void ElevatorData::updateCurrentState(double interval){
  if(isBreakRegion()){
    if(isFullStop(interval)){
      currentVelocity = 0;
      currentPosition = getFloorPosition();
    }
    else{
      currentVelocity -= (currentVelocity > 0) ? interval*acceleration : -interval*acceleration;
    }
  } 
  else if(isAccelerateRegion()){
    doorCheck = 1;
    currentVelocity += (isElevatorBelow()) ? interval*acceleration : -interval*acceleration;
  }
  else if(isElevatorAtTargetFloor()){
    openAndCloseDoor();
  }
  currentPosition += currentVelocity * interval;
}

uint ElevatorData::getNumberOfRequiredLeds(){ return numberOfLeds; }

void ElevatorData::setNumberOfLeds(){
  numberOfLeds = floorLength*numberOfFloors + spaceLength *(numberOfFloors- 1); 
}

bool ElevatorData::isBreakRegion(){
  if(currentVelocity == 0){ return false; }
  if(!(isElevatorBelow() != !(currentVelocity > 0.0))){ //XOR -> !A != !B
     return true;
  }
  return (2*getDistance() <= currentVelocity*currentVelocity/acceleration);
}

bool ElevatorData::isAccelerateRegion(){
  return (absVelocity() < maximumVelocity && currentPosition != getFloorPosition());
}

double ElevatorData::absVelocity(){
  return (currentVelocity > 0) ? currentVelocity : -currentVelocity;
}

bool ElevatorData::isElevatorAbove(){
  return getFloorPosition() < currentPosition;
}

bool ElevatorData::isElevatorBelow(){
  return getFloorPosition() > currentPosition;
}

bool ElevatorData::isFullStop(double interval){
  return absVelocity() < interval*acceleration; 
}

bool ElevatorData::isElevatorAtTargetFloor(){
  return (!isElevatorAbove() && !isElevatorBelow() && currentVelocity == 0);
}

void ElevatorData::openAndCloseDoor(){
  if(doorCheck != 0){
  doorState += doorCheck;
    if(doorState >= 512){
      doorCheck = -1;
    }
    else if(doorState == 0){
      doorCheck = 0;
      targetReached();
    }
  }
}

double ElevatorData::getDistance(){
  return (currentPosition > getFloorPosition()) ? (currentPosition - getFloorPosition()) : (getFloorPosition() - currentPosition);
}

uint ElevatorData::getFloorPosition(){
  return (floorLength+spaceLength)*targetFloor;
}
