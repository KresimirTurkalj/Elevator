#include "ElevatorUnit.h"

void ElevatorUnit::updateForInterval(double milliseconds){  
  updateCurrentState(milliseconds);
  sendDataToStrip(currentPosition, getDoorPosition(), FLOOR_LENGTH);
}
