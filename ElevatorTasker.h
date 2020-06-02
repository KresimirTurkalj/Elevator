#ifndef ELEVATOR_TASKER_H
#define ELEVATOR_TASKER_H

#include "ElevatorUnit.h"
#include "Observation.h"

#define NUMBER_OF_UNITS 1
#define FIRST_PIN 2
#define INTERVAL 0.001

struct Task{
  uint targetFloor;
  Task* nextTask;
};

class ElevatorTasker: public Observer{
  public:
    ElevatorTasker();
    ~ElevatorTasker(); //moguće moraš novo
    virtual void assignTask(){}
    void addPendingTask(uint targetFloor);   
    void updateForInterval(double interval);
    void setParams();
  
  protected:
    Task *pendingTask;
    ElevatorUnit elevatorUnits[NUMBER_OF_UNITS];
 
    Task* popTask(Task *task);
    void deleteAllTasks();
    uint getNumberOfUnits();
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
