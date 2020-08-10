#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "Adafruit_NeoPixel.h"

#define STRIP_PER_UNIT    2
#define NUMBER_OF_LEDS    28 //Ako mijenjaš ovdje, moraš i u ElevatorData.h, preuredi da ne misliš o ovome
#define DOOR_SLIDE        255*(1-doorPosition), 255*doorPosition, 0 
#define MOVE_COLOR_LAST   decimalPart, 0, 0
#define MOVE_COLOR        255, 0, 0
#define MOVE_COLOR_FIRST  255-decimalPart, 0, 0
#define OFF_COLOR         0, 0, 0

typedef unsigned int uint;

class LedUnit{
   public:
    LedUnit(){}
    LedUnit(const LedUnit &obj);
    void setFirstPin(uint firstPin);
    void sendDataToStrip(double lowestPosition, double doorPosition, uint elevatorLength);

  private:
    Adafruit_NeoPixel ledStrips[STRIP_PER_UNIT];
};
#endif
