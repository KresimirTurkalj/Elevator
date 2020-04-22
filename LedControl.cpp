#include "LedControl.h"

LedStrip::LedStrip(Adafruit_NeoPixel strip){ 
  this->strip = strip;
  strip.begin();
  strip.setBrightness(50);
  strip.show();
}

uint32_t LedStrip::getColor(int Red, int Green, int Blue){
  return strip.Color(Red, Green, Blue);
}

void LedStrip::updateLEDs(double startingPoint, int arrayLength, uint32_t colorArray[]){
  strip.fill(strip.Color(OFF_COLOR));
  for(int i = 0; i < arrayLength+1;i++){
    strip.setPixelColor((int)startingPoint+i,colorArray[i]);
  }
  strip.show();
}

LedUnit::~LedUnit(){
  for(int i = 0; i < numberOfStrips; i++){
    delete (ledStrips+i);
  }
}

void LedUnit::sendDataToStrip(double lowestPosition, int elevatorLength, int doorState){
  uint32_t colorArray[9];
  if(doorState > 0){
    if(doorState > 256) return;
    for(int i = 0; i < elevatorLength; i++){ colorArray[i] = ledStrips->getColor(DOOR_SLIDE);}
    colorArray[elevatorLength] = ledStrips->getColor(OFF_COLOR);
  }
  else{
    int integerPart = (int)lowestPosition;
    int decimalPart = 1000 * (double)((lowestPosition - integerPart)/3.92);
    colorArray[0] = ledStrips->getColor(MOVE_COLOR_FIRST);
    for(int i = 1; i < elevatorLength+1; i++){ colorArray[i] = ledStrips->getColor(MOVE_COLOR); }
    colorArray[elevatorLength] = ledStrips->getColor(MOVE_COLOR_LAST);
  }
  for(int i = 0; i < numberOfStrips; i++){
    ledStrips[i].updateLEDs(lowestPosition, elevatorLength, colorArray);
  }
}
