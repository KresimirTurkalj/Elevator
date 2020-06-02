#include "ElevatorTasker.h"
#define PIN 10
ElevatorTasker* elevatorTasker;

void buttonPress(){
  uint targetFloor;
  //pročitaj odakle je i pošalji.
  elevatorTasker->addPendingTask(targetFloor);
}

void setup() {
  Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(PIN),buttonPress,RISING);
  elevatorTasker = ElevatorFactory::newInstance(ElevatorFactory::FIRST);
  elevatorTasker->setParams();
  elevatorTasker->addPendingTask(0);
  elevatorTasker->addPendingTask(1);
  elevatorTasker->addPendingTask(0);
}

void loop() {
  elevatorTasker->updateForInterval(0.001);
}
