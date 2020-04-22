#ifndef ELEVATOR_TASKER_H
#define ELEVATOR_TASKER_H

#include "ElevatorUnit.h"
#include "TaskObservation.h"

struct Task{
  uint targetFloor;
  Task* futureTask;
};

struct TaskerParams{
   uint numberOfUnits;
   uint numberOfStrips;
   uint stripsPerUnit;
   uint ledsPerStrip;
};

class ElevatorTasker: public TaskObserver{
  public:
  virtual void addTask(uint targetFloor);
  ElevatorTasker(TaskerParams taskerParams, ElevatorParams elevatorParams);
  ~ElevatorTasker();
  
  protected:
    Task** currentTasks;

    void addTaskAtUnit(uint targetFloor, uint unitNumber);
    void endTaskAtUnit(uint unitNumber);
    uint getNumberOfUnits();
    bool allElevatorsAreIdle();
    
  private:
    ElevatorUnit* elevatorUnits;
    uint numberOfUnits;
    
    void initCurrentTasks();
    void deleteFirstTaskAtUnit(uint unitNumber);
    void deleteAllTasks();
    ElevatorController();
};

class ElevatorTaskerFirst: public ElevatorTasker{
  public:
     ElevatorTaskerFirst(TaskerParams taskerParams,ElevatorParams elevatorParams):ElevatorTasker(taskerParams, elevatorParams){pendingTaskList = NULL;}
     ~ElevatorTaskerFirst();
     void addTask(uint targetFloor);
     void onTaskChanged();
     
  private:
    Task* pendingTaskList;

    void deleteFirstPendingTask();
};

class ElevatorFactory{
  public:
    static const int FIRST = 1;
    static const int DISTANCE = 2;
    static const int TIME = 3;
    static const int OPTIMAL = 4;
    
    static ElevatorTasker newInstance(int sort, TaskerParams taskerParams, ElevatorParams elevatorParams);
};

#endif
