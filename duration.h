#ifndef Duration_h
#define Duration_h

#include "Arduino.h"

#define HISTORY_SIZE 10

class Duration
{
  public:
    Duration();
    void start();
    void stop();
    void update();
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
    unsigned long _historyInMilli[HISTORY_SIZE];
    unsigned int _currentPositionInHistory;
    unsigned int _historySize;
    unsigned long _currentDurationInMilli;
    unsigned long _avgInMilli;
    bool _isRun;
    bool _isFirst;
};

#endif
