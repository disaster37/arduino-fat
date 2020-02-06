#ifndef Duration_h
#define Duration_h

#include "Arduino.h"

class Duration
{
  public:
    Duration();
    void start();
    void stop();
    void update();
    void setHistorySize(unsigned int size);
    unsigned long getCurrentValueInMilli();
    unsigned long getCurrentValueInSecond();
    unsigned long getCurrentValueInMinute();
    unsigned long getAvgFromHistoryInMilli();
    unsigned long getAvgFromHistoryInSecond();
    unsigned long getAvgFromHistoryInMinute();
    unsigned long getLastValueFromHistoryInMilli();
    unsigned long getLastValueFromHistoryInSecond();
    unsigned long getLastValueFromHistoryInMinute();
    
  private:
    unsigned long _currentTime;
    unsigned long _historyInMilli[];
    unsigned int _currentPoisitionInHistory;
    unsigned int _historySize;
    unsigned long _currentDurationInMilli;
    unsigned long _avgInMilli;
    bool _isRun;
};

#endif
