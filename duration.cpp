#include "duration.h"
#include "Arduino.h"



/**
 * Constructor
 */
Duration::Duration()
{
    _currentTime = 0;
    _currentPositionInHistory = 0;
    _historySize = 0;
    _currentDurationInMilli = 0;
    _avgInMilli = 0;
    _isRun = false;
}

void Duration::start() {
    _currentTime = millis();
    _isRun = true;
    _currentDurationInMilli = 0;
}

void Duration::stop() {
    
    if(_historySize > 0) {
        if(_currentPositionInHistory + 1 == _historySize) {
            _currentPositionInHistory = 0;
        }
        else {
            _currentPositionInHistory = _currentPositionInHistory + 1;
        }
        
        _historySize[_currentPositionInHistory] = _currentDurationInMilli;
    }
    
    _currentTime = 0;
    _isRun = false;
}

void Duration::update() {
    if(_isRun == true) {
        _currentDurationInMilli = millis() - _currentTime;
        
        unsigned long avg = 0;
        unsigned int nbItem = 0;
    
        for(int i = 0; i < _historySize; i++) {
            if(_historyInMilli[i] > 0) {
                avg = avg + _historyInMilli[i]
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
}

void Duration::setHistorySize(unsigned int size) {
    _historyInMilli[] = unsigned int[size];
    _historySize = size;
    
    for(i = 0; i < size; i++) {
        _historyInMilli[i] = 0;
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
    return (_avgInMinute / 60000);
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