#ifndef ELEVATOR_TASKER_H
#define ELEVATOR_TASKER_H

#include "ElevatorUnit.h"
#include "Observation.h"

struct TaskerParams{
   uint numberOfUnits;
   uint numberOfStrips;
   uint stripsPerUnit;
   uint ledsPerStrip;
};

class ElevatorTasker: public TaskObserver{
  public:
    ElevatorTasker(TaskerParams taskerParams, ElevatorParams elevatorParams);
    ~ElevatorTasker();
    virtual void addTaskToUnit(){}
    void addPendingTask(uint targetFloor);   
    void updateForInterval(double interval);
    void setObservers();
  
  protected:
    Task* pendingTaskFirst;
    ElevatorUnit* elevatorUnits;

    void addTaskAtUnit(uint targetFloor, uint unitNumber); 
    void deleteFirstPendingTask();
    void deleteAllPendingTasks();
    uint getNumberOfUnits();
    bool allElevatorsAreIdle();
    
  private:
    uint numberOfUnits;
};

class ElevatorTaskerFirst: public ElevatorTasker{
  public:
     ElevatorTaskerFirst(TaskerParams taskerParams, ElevatorParams elevatorParams):ElevatorTasker(taskerParams, elevatorParams){}
     void addTaskToUnit();
};

class ElevatorFactory{
  public:
    static const int FIRST = 1;
    static const int DISTANCE = 2;
    static const int TIME = 3;
    static const int OPTIMAL = 4;
    
    static ElevatorTasker* newInstance(int sort, TaskerParams taskerParams, ElevatorParams elevatorParams);
};

#endif
