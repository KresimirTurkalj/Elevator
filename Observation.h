#ifndef OBSERVATION_H
#define OBSERVATION_H

#include <Arduino.h>

#define NULL 0
#define DEBUG

class Observer{
  public:
    Observer(){}
    virtual void taskFinished(unsigned int uintNum) = 0;
};

class Observable{
  public:
      unsigned int unitNum;
  
     Observable(){}
     void setInfo(Observer* obs, int num) { observer = obs; unitNum = num; }
  protected:
    void taskEnded(){
      observer->taskFinished(unitNum);
      }
  private:
     Observer* observer;
};

#endif
