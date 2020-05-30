#ifndef ELEVATOR_UNIT_H
#define ELEVATOR_UNIT_H

#include "ElevatorData.h"
#include "LedControl.h"
#include "Observation.h"



class ElevatorUnit : public ElevatorData, public LedUnit {
  public:
    ElevatorUnit():ElevatorData(), LedUnit(){}
    void updateForInterval(double milliseconds);
};

#endif
