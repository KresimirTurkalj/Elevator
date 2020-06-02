#include "ElevatorTasker.h"

ElevatorTasker::ElevatorTasker(): Observer(){
  for(int i = 0; i < MAX_TASKS; i++){
    pendingTasks[i] = EMPTY;
  }
  lastTaskIndex = EMPTY;
}

void ElevatorTasker::addPendingTask(uint targetFloor){
  if(pendingTasksFull()){
    lastTaskIndex++;
    pendingTasks[lastTaskIndex] = targetFloor; 
  }
  assignTask();
}

void ElevatorTasker::updateForInterval(double interval){
  for(int i = 0; i < NUMBER_OF_UNITS; i++){
    elevatorUnits[i].updateForInterval(interval);
  }
}

void ElevatorTasker::setParams() {
    for (int i = 0; i < NUMBER_OF_UNITS; i++){
        elevatorUnits[i].setObserver(this);
        elevatorUnits[i].setFirstPin(2+NUMBER_OF_UNITS*i);
    }
}

void ElevatorTasker::popTask(){ //Ako se izvede preko klase, neće trebati argument i povratna
  if(pendingTasksFull()){
    for(int i = 0; i < lastTaskIndex; i++){
      pendingTasks[i] = pendingTasks[i+1];
    }
    pendingTasks[lastTaskIndex] = EMPTY;
    lastTaskIndex--;
  }
}

void ElevatorTaskerFirst::assignTask(){
  for(int i = 0; i < NUMBER_OF_UNITS; i++){
    if(pendingTasksEmpty()){
      if(elevatorUnits[i].isIdle()){
        elevatorUnits[i].setTargetFloor(pendingTasks[0]);
        popTask();
      }
    }
  }
}

bool ElevatorTasker::pendingTasksFull(){
  return lastTaskIndex < MAX_TASKS;
}

bool ElevatorTasker::pendingTasksEmpty(){
  return lastTaskIndex > EMPTY;
}


ElevatorTasker* ElevatorFactory::newInstance(int sort){
   switch(sort){
    case FIRST:
      return new ElevatorTaskerFirst();
    case DISTANCE:
      //return new ElevatorTaskerDistance(taskerParams, elevatorParams);
    case TIME:
      //return new ElevatorTaskerTime(taskerParams, elevatorParams);
    case OPTIMAL:
      //return new ElevatorTaskerOptimal(taskerParams, elevatorParams);
    default:
      return new ElevatorTaskerFirst();
  }
}
