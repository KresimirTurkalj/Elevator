#ifndef ELEVATOR_UNIT_H
#define ELEVATOR_UNIT_H

#include "ElevatorData.h"
#include "LedControl.h"

class ElevatorUnit : public ElevatorData, public LedUnit {
  public:
    ElevatorUnit(): ElevatorData(ElevatorParams{0, 0, 0, 0, 0}), LedUnit(0,0){}
    ElevatorUnit(ElevatorParams elevatorParams, LedStrip* ledStrips,uint numberOfStrips): ElevatorData(elevatorParams), LedUnit(ledStrips, numberOfStrips){}
    void updateForInterval(double milliseconds);
};

#endif
