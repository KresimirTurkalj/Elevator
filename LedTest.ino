#include "ElevatorTasker.h"
ElevatorTasker* elevatorTasker;

void setup() {
  elevatorTasker = ElevatorFactory::newInstance(ElevatorFactory::FIRST);
  elevatorTasker->setParams();
  //elevatorUnit.setFirstPin(2);
  //elevatorUnit.setTargetFloor(0);
  elevatorTasker->addPendingTask(1);
  elevatorTasker->addPendingTask(0);
}

void loop() {
  elevatorTasker->updateForInterval(0.001);
}
