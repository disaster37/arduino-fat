#ifndef Timer_h
#define Timer_h

#include "Arduino.h"

class Timer
{
  public:
    Timer();
    void setTimerInSecond(unsigned long time);
    void setTimerInMilli(unsigned long time);
    unsigned long getCurrentValueInSecond();
    unsigned long getCurrentValueInMilli();
    bool isJustFinished();
    bool isRun();
    void start();
    void update();
  private:
    unsigned long _currentTime;
    unsigned long _timerValue;
    bool _isRun;
    bool _isJustFinished;
    unsigned long _currentTimer;
};

#endif
