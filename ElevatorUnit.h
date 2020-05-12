#ifndef ELEVATOR_UNIT_H
#define ELEVATOR_UNIT_H

#include "ElevatorData.h"
#include "LedControl.h"
#include "Observation.h"

struct Task{
  uint targetFloor;
  Task* futureTask;
};

class ElevatorUnit : public ElevatorData, public LedUnit, public TargetObserver,public TaskObservable {
  public:
    ElevatorUnit(): ElevatorData(), LedUnit(), TargetObserver(), TaskObservable(){} //samo kako bih instacirao polje
    ElevatorUnit(ElevatorParams elevatorParams, UnitParams unitParams) : ElevatorData(elevatorParams), LedUnit(unitParams), TargetObserver(), TaskObservable(), currentTask(NULL) {}
    ~ElevatorUnit();
    void updateForInterval(double milliseconds);
    void addTask(uint targetFloor);
    void deleteAllTasks();
    bool isIdle();
    void removeCurrentTask();
    void changeTarget();
    void setObserver(TaskObserver* observer);

  private:
    Task* currentTask;
    
    void removeFirstTask();
};

#endif
