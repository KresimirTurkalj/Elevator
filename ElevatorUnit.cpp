#include "ElevatorUnit.h"

ElevatorUnit::~ElevatorUnit(){ deleteAllTasks(); }

void ElevatorUnit::updateForInterval(double milliseconds){
  updateCurrentState(milliseconds);
  sendDataToStrip(currentPosition, floorLength, doorState);
}

void ElevatorUnit::addTask(uint targetFloor){
  Task *task, *newTask = new Task;
  newTask->targetFloor = targetFloor;
  newTask->futureTask = NULL;
  if(currentTask == NULL){ currentTask = newTask; }
  else{
  	task = currentTask;
  	while(task->futureTask != NULL){
    	task = task->futureTask;
  	}
  	task->futureTask = newTask;
  }
  if(currentTask == newTask){ changeTarget(); }
}
void ElevatorUnit::removeFirstTask(){
  Task* task = currentTask;
  currentTask = currentTask->futureTask;
  delete task;
  if(currentTask != NULL){ changeTarget(); }
  else{ taskEnded(); }
}

void ElevatorUnit::deleteAllTasks(){
  while(currentTask != NULL){
    removeFirstTask();
  }
}
bool ElevatorUnit::isIdle(){
  return currentTask == NULL;
}

void ElevatorUnit::removeCurrentTask(){
  removeFirstTask();
}

void ElevatorUnit::changeTarget(){
	setTargetFloor(currentTask->targetFloor);
}

void ElevatorUnit::setObserver(TaskObserver* observer){
    TaskObservable::setObserver(observer);
    ElevatorData::setObserver(this);
}
