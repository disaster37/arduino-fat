#include "duration.h"
#include "Arduino.h"



/**
 * Constructor
 */
Duration::Duration()
{
    _currentTime = 0;
    _currentPositionInHistory = 0;
    _historySize = HISTORY_SIZE;
    _currentDurationInMilli = 0;
    _avgInMilli = 0;
    _isRun = false;
    _isFirst = true;
}

void Duration::start() {
    _currentTime = millis();
    _isRun = true;
    _currentDurationInMilli = 0;
}

void Duration::stop() {

    if(_isRun) {


      // Update history
      if(_historySize > 0) {
          // The first item
          if(_isFirst) {
            _historyInMilli[_currentPositionInHistory] = _currentDurationInMilli;
            _isFirst = false;
          }
          else {
            if(_currentPositionInHistory + 1 == _historySize) {
                _currentPositionInHistory = 0;
            }
            else {
                _currentPositionInHistory = _currentPositionInHistory + 1;
            }
            _historyInMilli[_currentPositionInHistory] = _currentDurationInMilli;
          }
      }

      // Process average
      unsigned long avg = 0;
      unsigned int nbItem = 0;
    
      for(int i = 0; i < _historySize; i++) {
          if(_historyInMilli[i] > 0) {
              avg = avg + _historyInMilli[i];
              nbItem = nbItem + 1;
          }
          else {
              break;
          }
      }
      
      if(nbItem == 0) {
         _avgInMilli = 0;
      }
      else {
          _avgInMilli =  avg / nbItem;
      }
    }
    
    _currentTime = 0;
    _isRun = false;
}

void Duration::update() {
    if(_isRun == true) {
        _currentDurationInMilli = millis() - _currentTime;
    }
}

unsigned long Duration::getCurrentValueInMilli() {
    return _currentDurationInMilli;
}

unsigned long Duration::getCurrentValueInSecond() {
    return (_currentDurationInMilli / 1000);
}

unsigned long Duration::getCurrentValueInMinute() {
    return (_currentDurationInMilli / 60000);
}

unsigned long Duration::getAvgFromHistoryInMilli() {
    return _avgInMilli;
}

unsigned long Duration::getAvgFromHistoryInSecond() {
    return (_avgInMilli / 1000);
}

unsigned long Duration::getAvgFromHistoryInMinute() {
    return (_avgInMilli / 60000);
}

unsigned long Duration::getLastValueFromHistoryInMilli() {
    if(_historySize > 0) {
        return _historyInMilli[_currentPositionInHistory];
    }
    else {
        return 0;
    }
}

unsigned long Duration::getLastValueFromHistoryInSecond() {
    return (getLastValueFromHistoryInMilli() / 1000);
}

unsigned long Duration::getLastValueFromHistoryInMinute() {
    return (getLastValueFromHistoryInMilli() / 60000);
}
