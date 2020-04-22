#include "ElevatorTasker.h"

ElevatorTasker::ElevatorTasker(TaskerParams taskerParams,ElevatorParams elevatorParams): TaskObserver(){
  numberOfUnits = taskerParams.numberOfUnits;
  elevatorUnits = new ElevatorUnit[numberOfUnits];
  LedStrip* ledStrips = new LedStrip[taskerParams.numberOfStrips]; 
  for(int i = 0; i < taskerParams.numberOfStrips; i++){
    ledStrips[i] = LedStrip(Adafruit_NeoPixel(taskerParams.ledsPerStrip, i+2, NEO_GRB + NEO_KHZ800)); //koristimo digitalne pinove 2-13
  }
  LedStrip* ledStripPart;
  for(int i = 0; i < taskerParams.numberOfUnits; i++){ //Pretpostavka da je numberOfStrips višekratnik od numberOfUnits po bazi stripsPerUnit
    ledStripPart = ledStrips+i*taskerParams.stripsPerUnit;
    elevatorUnits[i] = ElevatorUnit(elevatorParams, ledStripPart,taskerParams.stripsPerUnit);
  }
  initCurrentTasks();
}

ElevatorTasker::~ElevatorTasker(){
  deleteAllTasks();
  delete[] elevatorUnits;
}

void ElevatorTasker::addTaskAtUnit(uint targetFloor, uint unitNumber){
  Task* nextTask = currentTasks[unitNumber];
  while(nextTask != NULL){
    nextTask = nextTask->futureTask;
  }
  nextTask = new Task;
  nextTask->futureTask = NULL;
  nextTask->targetFloor = targetFloor;
  if(currentTasks[unitNumber] == nextTask){ onTaskChanged(); }
}

void ElevatorTasker::endTaskAtUnit(uint unitNumber){
  deleteFirstTaskAtUnit(unitNumber);
  onTaskChanged();
}

uint ElevatorTasker::getNumberOfUnits(){
  return numberOfUnits;  
}

bool ElevatorTasker::allElevatorsAreIdle(){
  for(int i = 0; i <numberOfUnits; i++){
    if(currentTasks[i] != NULL){return false;}
  }
  return true;
}

void ElevatorTasker::initCurrentTasks(){
    currentTasks = new Task*[numberOfUnits];
    for(int i = 0; i < numberOfUnits; i++){ currentTasks[i] = NULL; }
}

void ElevatorTasker::deleteFirstTaskAtUnit(uint unitNumber){
  Task* tempTask = currentTasks[unitNumber];
  currentTasks[unitNumber] = currentTasks[unitNumber]->futureTask;
  delete tempTask;  
}

void ElevatorTasker::deleteAllTasks(){ //ISPRAVI
  for(int i = 0; i < numberOfUnits; i++){
    while(currentTasks[i] != NULL){
      deleteFirstTaskAtUnit(i);
    }
  }
  delete[] currentTasks;
}

ElevatorTaskerFirst::~ElevatorTaskerFirst(){
  int i = 0;
  while(currentTasks[i] != NULL){
    deleteFirstPendingTask();
    i++;
  }
}

void ElevatorTaskerFirst::addTask(uint targetFloor){
  Task* newTask = pendingTaskList;
  while(newTask != NULL){
    newTask = newTask->futureTask;
  }
  newTask = new Task;
  newTask->targetFloor = targetFloor;
  newTask->futureTask = NULL;
  if(allElevatorsAreIdle()){ onTaskChanged(); }
}

void ElevatorTaskerFirst::onTaskChanged(){
  int i = 0;
  while(currentTasks[i] != NULL || i >= getNumberOfUnits()){ i++; }
  currentTasks[i] = pendingTaskList;
  deleteFirstPendingTask();
}

void ElevatorTaskerFirst::deleteFirstPendingTask(){
  Task* tempTask = pendingTaskList;
  pendingTaskList = pendingTaskList->futureTask;
  delete tempTask; 
}

static ElevatorTasker ElevatorFactory::newInstance(int sort, TaskerParams taskerParams,ElevatorParams elevatorParams){
      switch(sort){
        case FIRST:
          return ElevatorTaskerFirst(taskerParams, elevatorParams);
        case DISTANCE:
          //return ElevatorTaskerDistance(taskerParams, elevatorParams);
        case TIME:
          //return ElevatorTaskerTime(taskerParams, elevatorParams);
        case OPTIMAL:
          //return ElevatorTaskerOptimal(taskerParams, elevatorParams);
        default:
          return ElevatorTaskerFirst(taskerParams, elevatorParams);
    }
  }
