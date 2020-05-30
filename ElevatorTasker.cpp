#include "ElevatorTasker.h"

ElevatorTasker::ElevatorTasker(): Observer(){
  assignedTasks = new Task*[NUMBER_OF_UNITS];
  for(int i = 0; i < NUMBER_OF_UNITS; i++){
    assignedTasks[i] = NULL;
  }
  pendingTasks = NULL;
}

ElevatorTasker::~ElevatorTasker(){
  deleteAllTasks();
}

void ElevatorTasker::addPendingTask(uint targetFloor){
  Task *task, *newTask = new Task{targetFloor, NULL};
  if(pendingTasks == NULL){ pendingTasks = newTask; }
  else{
    task = pendingTasks;
    while(task->nextTask != NULL){
      task = task->nextTask;
    }
    task->nextTask = newTask;
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

Task* ElevatorTasker::popTask(Task *task){ //Ako se izvede preko klase, neće trebati argument i povratna
  Task *tempTask = task;
  task = task->nextTask;
  delete tempTask;
  return task;
}

void ElevatorTasker::deleteAllTasks(){
  Task tempTask;
  while(pendingTasks != NULL){
    pendingTasks = popTask(pendingTasks);
  }
  for(int i = 0; i < NUMBER_OF_UNITS; i++){
    while(assignedTasks[i] != NULL)
    assignedTasks[i] = popTask(assignedTasks[i]);
  }
  delete assignedTasks;
}

void ElevatorTaskerFirst::assignTask(){
  if(pendingTasks != NULL){
    for(int i = 0; i < NUMBER_OF_UNITS; i++){
      if(elevatorUnits[i].isIdle()){
        elevatorUnits[i].setTargetFloor(pendingTasks->targetFloor);
        pendingTasks = popTask(pendingTasks);
      }
    }
  }
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
