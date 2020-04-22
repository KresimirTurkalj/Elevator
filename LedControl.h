#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Adafruit_NeoPixel.h>

#define MAX_LED 92
#define LED_PIN 6
#define DELAY 5
#define DOOR_SLIDE        255-doorState, doorState, 0
#define MOVE_COLOR_LAST   decimalPart, 0, 0
#define MOVE_COLOR        255, 0, 0
#define MOVE_COLOR_FIRST  255-decimalPart, 0, 0
#define OFF_COLOR         0, 0, 0

#include <stdint.h>

class LedStrip{
  public:
    LedStrip(Adafruit_NeoPixel strip);
    uint32_t getColor(int Red, int Green, int Blue);
    void updateLEDs(double startingPoint, int arrayLength, uint32_t colorArray[]);
    LedStrip(){}

  private:
    Adafruit_NeoPixel strip;
};

class LedUnit{
  public:
    LedUnit(LedStrip* ledStrips, int numberOfStrips): numberOfStrips(numberOfStrips), ledStrips(ledStrips){}
    ~LedUnit();
    
  protected: 
    void sendDataToStrip(double lowestPosition, int elevatorSize, int doorState);

  private:
    int numberOfStrips;
    LedUnit();
    LedStrip* ledStrips;
};
#endif
