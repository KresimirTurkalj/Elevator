#include "ElevatorTasker.h"

ElevatorTasker::ElevatorTasker(TaskerParams taskerParams,ElevatorParams elevatorParams): TaskObserver(){
  numberOfUnits = taskerParams.numberOfUnits;
  elevatorUnits = new ElevatorUnit[numberOfUnits];
  pendingTaskFirst = NULL;
  for(uint i = 0; i < taskerParams.numberOfUnits; i++){ //Jednako podijeli stripove i počne od pin 2
    elevatorUnits[i] = ElevatorUnit(elevatorParams, UnitParams{i+2, taskerParams.stripsPerUnit});
  }
}

ElevatorTasker::~ElevatorTasker(){
  deleteAllPendingTasks();
  delete[] elevatorUnits;
}

void ElevatorTasker::addPendingTask(uint targetFloor){
  Task *task, *newTask = new Task;
  newTask->targetFloor = targetFloor;
  newTask->futureTask = NULL;
  if(pendingTaskFirst == NULL){ pendingTaskFirst = newTask; }
  else{
  	task = pendingTaskFirst;
  	while(task->futureTask != NULL){
    	task = task->futureTask;
  	}
  	task->futureTask = newTask;
  }
  addTaskToUnit();
}

void ElevatorTasker::updateForInterval(double interval){
  for(int i = 0; i < numberOfUnits; i++){
    elevatorUnits[i].updateForInterval(interval);
  }
}

void ElevatorTasker::setObservers() {
    for (int i = 0; i < numberOfUnits; i++){
        elevatorUnits[i].setObserver(this);
    }
}

void ElevatorTasker::addTaskAtUnit(uint targetFloor, uint unitNumber){
  elevatorUnits[unitNumber].addTask(targetFloor);
}

void ElevatorTasker::deleteFirstPendingTask(){
  Task* tempTask = pendingTaskFirst;
  pendingTaskFirst = pendingTaskFirst->futureTask;
  delete tempTask; 
}

void ElevatorTasker::deleteAllPendingTasks(){
  while(pendingTaskFirst != NULL){
    deleteFirstPendingTask();
  }
}

uint ElevatorTasker::getNumberOfUnits(){
  return numberOfUnits;  
}

bool ElevatorTasker::allElevatorsAreIdle(){
  for(int i = 0; i <numberOfUnits; i++){
    if(!elevatorUnits[i].isIdle()) return false;
  }
  return true;
}

void ElevatorTaskerFirst::addTaskToUnit(){
  for(int i = 0; i < getNumberOfUnits(); i++){
    if(pendingTaskFirst == NULL) {return;}
    if(elevatorUnits[i].isIdle()){
      elevatorUnits[i].addTask(pendingTaskFirst->targetFloor);
      deleteFirstPendingTask();
    }
  }
}

ElevatorTasker* ElevatorFactory::newInstance(int sort, TaskerParams taskerParams,ElevatorParams elevatorParams){
   switch(sort){
    case FIRST:
      return new ElevatorTaskerFirst(taskerParams, elevatorParams);
    case DISTANCE:
      //return new ElevatorTaskerDistance(taskerParams, elevatorParams);
    case TIME:
      //return new ElevatorTaskerTime(taskerParams, elevatorParams);
    case OPTIMAL:
      //return new ElevatorTaskerOptimal(taskerParams, elevatorParams);
    default:
      return new ElevatorTaskerFirst(taskerParams, elevatorParams);
  }
}
