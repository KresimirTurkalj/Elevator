#ifndef TASK_OBSERVABLE_H
#define TASK_OBSERVABLE_H

class TaskObservable{
  public:
     TaskObservable(){}
  private:
     virtual void onTaskCompleted() = 0;    // "= 0" part makes this method pure virtual, and also makes this class abstract.
};

class TaskObserver{
  public:
    TaskObserver(){}
    virtual void onTaskChanged();
};

#endif
