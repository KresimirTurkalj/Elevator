#ifndef ELEVATOR_TASKER_H
#define ELEVATOR_TASKER_H

#include "ElevatorUnit.h"
#include "Observation.h"

#define NUMBER_OF_UNITS 2
#define FIRST_PIN 3
#define MAX_DISTANCE 28

class ElevatorTasker : public Observer {
public:
    ElevatorTasker();
    virtual void assignTask() {}
    void taskFinished(uint unitNum);
    void updateForInterval(double interval);
    void setParams();
    void addPendingTask(uint targetFloor);

protected:
    int pendingTasks[MAX_TASKS];
    int assignedTasks[MAX_TASKS][NUMBER_OF_UNITS];
    ElevatorUnit elevatorUnits[NUMBER_OF_UNITS];
    int lastTaskIndex;

    void popPendingTask();
    bool pendingTasksFull();
    bool assignedTasksFull(uint unitNum);
    bool assignedTasksEmpty(uint unitNum);
    bool pendingTasksEmpty();
    virtual void addAssignedTask(uint targetFloor, uint unitNum);
    bool isTaskAlreadyAdded(uint targetFloor);
    void popAssignedTask(uint unitNum);
    void startNewTask(uint unitNum);
};

class ElevatorTaskerFirst : public ElevatorTasker {
public:
    ElevatorTaskerFirst() :ElevatorTasker() {}
    void assignTask();
};

class ElevatorTaskerDistance : public ElevatorTasker {
public:
    ElevatorTaskerDistance() :ElevatorTasker() {}
    void assignTask();
};

class ElevatorTaskerTime : public ElevatorTasker {
public:
    ElevatorTaskerTime() :ElevatorTasker() {}
    void assignTask();
protected:
    struct TimeInfo{
        double timeNeeded;
        uint position;
    };
    TimeInfo getTimeInfo(int unitNum);
    void addAssignedTask(uint targetFloor, uint unitNum, uint position);
};

class ElevatorFactory {
public:
    static const int FIRST = 1;
    static const int DISTANCE = 2;
    static const int TIME = 3;
    static const int OPTIMAL = 4;

    static ElevatorTasker* newInstance(int sort);
};

#endif
