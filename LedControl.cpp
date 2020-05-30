#include "LedControl.h"

void LedUnit::setFirstPin(uint firstPin){
  for(int i = 0; i < NUMBER_OF_STRIPS; i++){
    ledStrips[i] = Adafruit_NeoPixel(NUMBER_OF_LEDS, firstPin+i, NEO_GRB + NEO_KHZ800);
    ledStrips[i].begin();
    ledStrips[i].setBrightness(100);
  }
}

LedUnit::LedUnit(const LedUnit &obj){
  for(int i = 0; i < NUMBER_OF_STRIPS; i++){
    ledStrips[i] = obj.ledStrips[i];
  }
}

void LedUnit::sendDataToStrip(double lowestPosition, double doorPosition, uint elevatorLength){
  for(int i = 0; i < NUMBER_OF_STRIPS; i++){
    ledStrips[i].clear();
    if(doorPosition > 0){
        for(int j = 0; j < elevatorLength; j++){
          ledStrips[i].setPixelColor(lowestPosition+j,Adafruit_NeoPixel::Color(DOOR_SLIDE));
        } 
    }
    else{
      int integerPart = (int)lowestPosition;
      int decimalPart = 1000 * (double)((lowestPosition - integerPart)/3.92);   //<0,1> -> <0,255]
      ledStrips[i].setPixelColor(integerPart, Adafruit_NeoPixel::Color(MOVE_COLOR_FIRST));
      for(int j = 1; j < elevatorLength; j++){ 
        ledStrips[i].setPixelColor(integerPart+j, Adafruit_NeoPixel::Color(MOVE_COLOR));
      }
      ledStrips[i].setPixelColor(integerPart+elevatorLength,Adafruit_NeoPixel::Color(MOVE_COLOR_LAST));
    }
    ledStrips[i].show();
  }
}
