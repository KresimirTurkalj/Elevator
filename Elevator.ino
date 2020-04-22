#include "ElevatorTasker.h"

#define INTERRUPT_PIN 

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
ElevatorTasker elevator = ElevatorFactory::newInstance(ElevatorFactory::FIRST, taskerParams, elevatorParams);

void onButtonPressed(){
  
}

void setup() {
  //attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), onButtonPressed, RISING)
  
}
void loop() {
  
}
