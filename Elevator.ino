#include "ElevatorTasker.h"
#define PIN 2
ElevatorTasker* elevatorTasker;
int currentTask = 0;

void buttonPress(){
  elevatorTasker->addPendingTask(++currentTask);
}

void setup() {
  //pinMode(PIN,INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(PIN),buttonPress,RISING);
  elevatorTasker = ElevatorFactory::newInstance(ElevatorFactory::DISTANCE);
  elevatorTasker->setParams();
  for(int i = 1; i < 4; i++){
    elevatorTasker->addPendingTask(i);
  }
  elevatorTasker->addPendingTask(3);
}

void loop() {
  elevatorTasker->updateForInterval(0.001);
}
