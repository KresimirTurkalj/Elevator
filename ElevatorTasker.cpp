#include "ElevatorTasker.h"

ElevatorTasker::ElevatorTasker(): Observer(){
  pendingTask = NULL;
}

ElevatorTasker::~ElevatorTasker(){
  deleteAllTasks();
}

void ElevatorTasker::addPendingTask(uint targetFloor){
  Task *task, *newTask = new Task{targetFloor, NULL};
  Serial.println("New task!");
  Serial.print("Memory: ");
  Serial.print((int)newTask);
  Serial.print("\tTarget: ");
  Serial.println(newTask->targetFloor);
  if(pendingTask == NULL){ pendingTask = newTask; }
  else{
    task = pendingTask;
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
  Serial.println("Pop!");
  Serial.print("Memory: ");
  Serial.print((int)task);
  Task *tempTask = task;
  task = task->nextTask;
  Serial.print("\tMemory: ");
  Serial.println((int)task);
  delete tempTask;
  return task;
}

void ElevatorTasker::deleteAllTasks(){
  Task tempTask;
  while(pendingTask != NULL){
    pendingTask = popTask(pendingTask);
  }
}

void ElevatorTaskerFirst::assignTask(){
  Serial.println("Tasks!");
  Task *task = pendingTask;
  while(task != NULL){
    Serial.print("Memory: ");
    Serial.print((int)task);
    Serial.print("\tTarget: ");
    Serial.println(task->targetFloor);
    task = task->nextTask;
  }
  if(pendingTask != NULL){
    for(int i = 0; i < NUMBER_OF_UNITS; i++){
      if(elevatorUnits[i].isIdle()){
        Serial.println("Task assigned!");
        Serial.print("Memory: ");
        Serial.print((int)pendingTask);
        Serial.print("\tTarget: ");
        Serial.println(pendingTask->targetFloor);
        elevatorUnits[i].setTargetFloor(pendingTask->targetFloor);
        pendingTask = popTask(pendingTask);
      }
    }
  }
  Serial.println("Tasks! after");
    task = pendingTask; 
    while(task != NULL){
      Serial.print("Memory: ");
      Serial.print((int)task);
      Serial.print("\tTarget: ");
      Serial.println(task->targetFloor);
      task = task->nextTask;
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
