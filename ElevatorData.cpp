#include "ElevatorData.h"

ElevatorData::ElevatorData() : Observable() {
    currentPosition = 0.0;
    currVeloc = 0.0;
    targetFloor = GROUND_FLOOR;
    numberOfFloors = (NUMBER_OF_LEDS - FLOOR_LENGTH) / (FLOOR_LENGTH + SPACE_LENGTH);
    idleState = true;
    doorPosition = 0.0;
    doorStage = OPEN_STAGE;
    doorStall = 0;
}

void ElevatorData::setTargetFloor(uint nextFloor) {
    if (targetFloor >= 0 && targetFloor < numberOfFloors) {
        targetFloor = nextFloor;
        idleState = false;
    }
}

bool ElevatorData::isIdle() {
    return idleState;
}

double ElevatorData::getTimeToTargetFloor(uint targetFloor) {
    if (getAccDistance() > getDistanceToFloor(targetFloor)) { return currVeloc / ACC; }
    if (currVeloc == MAX_VEL) { return alreadyMaxTime(); }
    if (canNotReachMaxVelocity(targetFloor)) { return getPartTime(targetFloor); }
    return getFullTime(targetFloor);

}

double ElevatorData::getTimeToNextFloor(uint firstFloor, uint secondFloor) {
    if (getAccDistance() * 2.0 > getDistanceBetweenFloors(firstFloor,secondFloor)) { return sqrt(2.0 * getDistanceBetweenFloors(firstFloor, secondFloor) / ACC); }
    return 2.0 * MAX_VEL / ACC + (getDistanceBetweenFloors(firstFloor, secondFloor) - 2.0 * getAccDistance()) / MAX_VEL;
}

bool ElevatorData::canStopAt(uint nextFloor) {
    if (correctOrientation(nextFloor) && !passedBreakingPoint(nextFloor)) {

    }
    return false;
}

double ElevatorData::getDoorPosition() {
    return doorPosition;
}

void ElevatorData::updateCurrentState(double interval) {
    if (!isIdle()) {
        if (isElevatorAtTargetFloor()) {
            moveDoor(interval);
        }
        else if (isFullStop(interval)) {
            currVeloc = 0;
            currentPosition = getFloorPosition();
        }
        else if (isBreakRegion()) {
            currVeloc -= (currVeloc > 0) ? interval * ACC : -interval * ACC;
        }
        else if (isAccelerateRegion()) {
            currVeloc += (isElevatorBelow()) ? interval * ACC : -interval * ACC;
        }
        currentPosition += currVeloc * interval;
    }
}

bool ElevatorData::isBreakRegion() {
    if (currVeloc == 0) { return false; }
    return (!correctOrientation(targetFloor) || passedBreakingPoint(targetFloor));
}

bool ElevatorData::isAccelerateRegion() {
    return (absVelocity() < MAX_VEL && currentPosition != getFloorPosition());
}

double ElevatorData::absVelocity() {
    return (currVeloc > 0.0) ? currVeloc : -currVeloc;
}

bool ElevatorData::isElevatorAbove() {
    return getFloorPosition() < currentPosition;
}

bool ElevatorData::isElevatorBelow() {
    return getFloorPosition() > currentPosition;
}

bool ElevatorData::correctOrientation(uint nextFloor) {
    return (getFloorPosition(nextFloor) > currentPosition) == (currVeloc > 0.0);
}

bool ElevatorData::passedBreakingPoint(uint nextFloor) {
    return getDistanceToFloor(nextFloor) <= currVeloc * currVeloc / (ACC * 2);
}

bool ElevatorData::isFullStop(double interval) { //Razmisli o Epsilon
    if (isElevatorBelow()) {
        return currentPosition + currVeloc * interval >= getFloorPosition();
    }
    else {
        return currentPosition + currVeloc * interval <= getFloorPosition();
    }
}

bool ElevatorData::isElevatorAtTargetFloor() {
    return (!isElevatorAbove() && !isElevatorBelow() && currVeloc == 0);
}

bool ElevatorData::canNotReachMaxVelocity(uint targetFloor)
{
    return currVeloc * (MAX_VEL - currVeloc) / ACC + MAX_VEL * MAX_VEL / (2 * ACC) + getAccDistance() > getDistanceToFloor(targetFloor);
}

void ElevatorData::moveDoor(double interval) { //razmisli da izvedeš s intervala, zasad prolazi.
    if (doorStage != STOP) {
        doorPosition += (double)(interval / DOOR_SLIDE_TIME * doorStage);
        if (doorPosition >= 1) {
            doorStage = STOP;
        }
        else if (doorPosition <= 0) {
            doorStall = 0;
            doorStage = OPEN_STAGE;
            idleState = true;
            taskEnded();
        }
    }
    else if (doorStall < 1) {
        doorStall += interval / DOOR_STOP_TIME;
    }
    else {
        doorStage = CLOSE_STAGE;
    }
}

double ElevatorData::getDistanceToFloor(uint nextFloor) {
    return (currentPosition > getFloorPosition(nextFloor)) ? (currentPosition - getFloorPosition(nextFloor)) : (getFloorPosition(nextFloor) - currentPosition);
}

double ElevatorData::getDistanceToFloor() {
    return getDistanceToFloor(targetFloor);
}

double ElevatorData::getAccDistance(){
    return MAX_VEL * MAX_VEL / (2 * ACC);
}

double ElevatorData::getDistanceBetweenFloors(int firstFloor, int secondFloor){
    return abs(firstFloor - secondFloor) * (FLOOR_LENGTH + SPACE_LENGTH);
}

double ElevatorData::alreadyMaxTime(){
    return (getDistanceToFloor() - getAccDistance()) / MAX_VEL + MAX_VEL / ACC;
}

double ElevatorData::getPartTime(uint targetFloor){
    return currVeloc / ACC + (sqrt(4.0 * ACC * getDistanceToFloor(targetFloor) + 2.0 * currVeloc) - 2.0 * currVeloc) / 2.0 * ACC;
}

double ElevatorData::getFullTime(uint targetFloor){
    return 2 * MAX_VEL / ACC + ((getDistanceToFloor(targetFloor) - 2 * getAccDistance() / MAX_VEL) - currVeloc / ACC);
}

double ElevatorData::getFloorPosition(uint nextFloor) {
    return (FLOOR_LENGTH + SPACE_LENGTH) * nextFloor;
}

double ElevatorData::getFloorPosition() {
    return getFloorPosition(targetFloor);
}
