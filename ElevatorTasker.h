#ifndef ELEVATOR_TASKER_H
#define ELEVATOR_TASKER_H

#include "ElevatorUnit.h"
#include "Observation.h"

#define NUMBER_OF_UNITS 1
#define FIRST_PIN 2
#define MAX_TASKS 10
#define EMPTY -1

class ElevatorTasker: public Observer{
  public:
    ElevatorTasker();
    virtual void assignTask(){}
    void addPendingTask(uint targetFloor);   
    void updateForInterval(double interval);
    void setParams();
  
  protected:
    int pendingTasks[MAX_TASKS];
    ElevatorUnit elevatorUnits[NUMBER_OF_UNITS];
    int lastTaskIndex;
    
    void popTask();
    uint getNumberOfUnits();
    bool pendingTasksFull();
    bool pendingTasksEmpty();
};

class ElevatorTaskerFirst: public ElevatorTasker{
  public:
     ElevatorTaskerFirst():ElevatorTasker(){}
     void assignTask();
};

class ElevatorFactory{
  public:
    static const int FIRST = 1;
    static const int DISTANCE = 2;
    static const int TIME = 3;
    static const int OPTIMAL = 4;
    
    static ElevatorTasker* newInstance(int sort);
};

#endif
