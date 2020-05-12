#ifndef OBSERVATION_H
#define OBSERVATION_H

#define NULL 0

class TaskObserver{
  public:
    TaskObserver(){}
    virtual void addTaskToUnit() = 0;
};

class TaskObservable{
  public:
     TaskObservable(): taskObserver(NULL){}
     virtual void setObserver(TaskObserver* observer) { this->taskObserver = observer; }
  protected:
    void taskEnded(){taskObserver->addTaskToUnit();}
  private:
     TaskObserver* taskObserver;
};

class TargetObserver {
public:
    TargetObserver() {}
    virtual void removeCurrentTask() = 0;
};

class TargetObservable {
public:
    TargetObservable() : targetObserver(NULL) {}
    virtual void setObserver(TargetObserver* observer) { this->targetObserver = observer; }
protected:
    void targetReached() { targetObserver->removeCurrentTask(); }
private:
    TargetObserver* targetObserver;
};

#endif
