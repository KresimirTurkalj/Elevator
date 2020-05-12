#include "ElevatorTasker.h"
#define INTERRUPT_PIN 1

uint numberOfStrips = 1;
uint numberOfUnits = 1;
uint stripsPerUnit = 1;
uint ledsPerStrip = 39;
uint floorLength = 4;
uint spaceLength = 2;
uint numberOfFloors = 6;
uint maxVelocity = 2.0;
double acceleration = 2.0;

TaskerParams taskerParams = TaskerParams{numberOfUnits, numberOfStrips, stripsPerUnit, ledsPerStrip};
ElevatorParams elevatorParams = ElevatorParams{floorLength, spaceLength, numberOfFloors, maxVelocity, acceleration};
ElevatorTasker* elevator;

unsigned long timeEnd = 0;

void onButtonPressed(){
  
}

void setup() {
  elevator = ElevatorFactory::newInstance(ElevatorFactory::FIRST, taskerParams, elevatorParams);
  elevator->setObservers();
  for(int i = 0; i < 3; i++){
    elevator->addPendingTask(3-i);
  }
  timeEnd = millis();
}
void loop() {
  elevator->updateForInterval(double((timeEnd-millis())/1000));
  timeEnd = millis();
}
