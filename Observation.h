#ifndef OBSERVATION_H
#define OBSERVATION_H

#define NULL 0

class Observer{
  public:
    Observer(){}
    virtual void assignTask() = 0;
};

class Observable{
  public:
     Observable(): observer(NULL){}
     virtual void setObserver(Observer* observer) { this->observer = observer; }
  protected:
    void taskEnded(){observer->assignTask();}
  private:
     Observer* observer;
};

#endif
