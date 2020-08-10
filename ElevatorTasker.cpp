#include "ElevatorTasker.h"

ElevatorTasker::ElevatorTasker() : Observer() {
    for (int i = 0; i < MAX_TASKS; i++) {
        pendingTasks[i] = EMPTY;
    }
    for (int i = 0; i < MAX_TASKS; i++) {
        for (int j = 0; j < NUMBER_OF_UNITS; j++) {
            assignedTasks[i][j] = EMPTY;
        }
    }
    lastTaskIndex = EMPTY;
}

void ElevatorTasker::taskFinished(uint unitNum){
  popAssignedTask(unitNum);
  startNewTask(unitNum);
}

void ElevatorTasker::updateForInterval(double interval) {
    for (int i = 0; i < NUMBER_OF_UNITS; i++) {
        elevatorUnits[i].updateForInterval(interval);
    }
}

void ElevatorTasker::setParams() {
    for (int i = 0; i < NUMBER_OF_UNITS; i++) {
        elevatorUnits[i].setInfo(this, i);
        elevatorUnits[i].setFirstPin(FIRST_PIN + STRIP_PER_UNIT * i);
    }
}

void ElevatorTasker::addPendingTask(uint targetFloor) {
    if (isTaskAlreadyAdded(targetFloor)) {
          return;
        }
    if (!pendingTasksFull()) {
        lastTaskIndex++;
        pendingTasks[lastTaskIndex] = targetFloor;
    }
    assignTask();
}

bool ElevatorTasker::isTaskAlreadyAdded(uint targetFloor) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (pendingTasks[i] == targetFloor) return true;
        for (int j = 0; j < NUMBER_OF_UNITS; j++) {
            if (assignedTasks[i][j] == targetFloor) { return true; }
        }
    }
    return false;
}

void ElevatorTasker::popPendingTask() {
    for (int i = 0; i < lastTaskIndex; i++) { pendingTasks[i] = pendingTasks[i + 1]; }
    pendingTasks[lastTaskIndex--] = EMPTY;
}

bool ElevatorTasker::pendingTasksFull() {
    return lastTaskIndex >= MAX_TASKS - 1;
}

bool ElevatorTasker::assignedTasksFull(uint unitNum)
{
    return assignedTasks[MAX_TASKS - 1][unitNum] != EMPTY;
}

bool ElevatorTasker::assignedTasksEmpty(uint unitNum)
{
    return assignedTasks[0][unitNum] == EMPTY;
}

bool ElevatorTasker::pendingTasksEmpty() {
    return lastTaskIndex <= EMPTY;
}

void ElevatorTasker::addAssignedTask(uint targetFloor, uint unitNum) {
    int i = 0;
    while (assignedTasks[i][unitNum] != EMPTY) { i++; }
    assignedTasks[i][unitNum] = targetFloor;
    startNewTask(unitNum);
}

void ElevatorTasker::popAssignedTask(uint unitNum) {
    for (int i = 0; i < MAX_TASKS - 1; i++) {
        assignedTasks[i][unitNum] = assignedTasks[i + 1][unitNum];
        if (assignedTasks[i][unitNum] == EMPTY) { return; }
    }
    assignedTasks[MAX_TASKS - 1][unitNum] = EMPTY;
}

void ElevatorTasker::startNewTask(uint unitNum) {
    if (!assignedTasksEmpty(unitNum)) {
        elevatorUnits[unitNum].setTargetFloor(assignedTasks[0][unitNum]);
    }
    else {
        assignTask();
    }
}

void ElevatorTaskerFirst::assignTask() {
    for (int i = 0; i < NUMBER_OF_UNITS; i++) {
        if (pendingTasksEmpty()) { return; }
        if (elevatorUnits[i].isIdle() && !assignedTasksFull(i)) {
            addAssignedTask(pendingTasks[0], i);
            popPendingTask();
        }
    }
}

void ElevatorTaskerDistance::assignTask() {
    uint selUnit = EMPTY;
    double distance = MAX_DISTANCE;
    if (pendingTasksEmpty()) { return; }
    for (int i = 0; i < NUMBER_OF_UNITS; i++) {
        if (elevatorUnits[i].isIdle() && !assignedTasksFull(i)) {
            if (distance > elevatorUnits[i].getDistanceToFloor(pendingTasks[0])) {
                distance = elevatorUnits[i].getDistanceToFloor(pendingTasks[0]);
                selUnit = i;
            }
        }
    }
    if (selUnit != EMPTY) {
        addAssignedTask(pendingTasks[0], selUnit);
        popPendingTask();
    }
}

void ElevatorTaskerTime::assignTask() {
    uint selUnit = EMPTY, position;
    double timeToFloor = MAX_TIME;
    if (pendingTasksEmpty()) { return; }
    for (int j = 0; j < NUMBER_OF_UNITS; j++) {
        if (assignedTasksFull(j)) { continue; }
        TimeInfo item = getTimeInfo(j);
        if (timeToFloor > item.timeNeeded) {
            timeToFloor = item.timeNeeded;
            selUnit = j;
            position = item.position;
        }
    }
    if (selUnit != EMPTY) {
        addAssignedTask(pendingTasks[0], selUnit, position);
        popPendingTask();
    }
}

ElevatorTaskerTime::TimeInfo ElevatorTaskerTime::getTimeInfo(int unitNum) {
    if (assignedTasks[0][unitNum] == EMPTY || elevatorUnits[unitNum].canStopAt(pendingTasks[0])) {
        return { elevatorUnits[unitNum].getTimeToTargetFloor(pendingTasks[0]), 0};
    }
    double timeNeeded = elevatorUnits[unitNum].getTimeToTargetFloor(assignedTasks[0][unitNum]) + DOOR_SLIDE_TIME * 2 + DOOR_STOP_TIME;
    uint pos = 1;
    while (assignedTasks[pos][unitNum] != EMPTY && pos < MAX_TASKS) {
        if (assignedTasks[pos - 1][unitNum] > pendingTasks[0] == assignedTasks[pos][unitNum] < pendingTasks[0]) { break; } // uvjet da je kat između dva zahtjeva
        timeNeeded += elevatorUnits[unitNum].getTimeToNextFloor(assignedTasks[pos-1][unitNum], assignedTasks[pos][unitNum]) + DOOR_SLIDE_TIME * 2 + DOOR_STOP_TIME;
        pos++;
    }
    return TimeInfo{ timeNeeded + elevatorUnits[unitNum].getTimeToNextFloor(assignedTasks[pos-1][unitNum], pendingTasks[0]), pos};
}

void ElevatorTaskerTime::addAssignedTask(uint targetFloor, uint unitNum, uint pos)
{
    for (int i = MAX_TASKS - 1; i > pos; i--){
        assignedTasks[i][unitNum] = assignedTasks[i-1][unitNum];
    }
    assignedTasks[pos][unitNum] = targetFloor;
    startNewTask(unitNum);
}

ElevatorTasker* ElevatorFactory::newInstance(int sort) {
    switch (sort) {
    case FIRST:
        return new ElevatorTaskerFirst();
    case DISTANCE:
        return new ElevatorTaskerDistance();
    case TIME:
        return new ElevatorTaskerTime();
    case OPTIMAL:
        //pointer = new ElevatorTaskerOptimal();
    default:
        return new ElevatorTaskerFirst();
    }
}
