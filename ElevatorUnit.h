#ifndef ELEVATOR_UNIT_H
#define ELEVATOR_UNIT_H

#include "ElevatorData.h"
#include "LedControl.h"
#include "Observation.h"

#define MAX_TASKS 10
#define EMPTY -1

class ElevatorUnit : public ElevatorData, public LedUnit {
  public:
    ElevatorUnit(): ElevatorData(), LedUnit(){}
    void updateForInterval(double milliseconds);
};

#endif
