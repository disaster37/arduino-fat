#include "timer.h"
#include "Arduino.h"



/**
 * Constructor
 */
Timer::Timer()
{
    _currentTime = 0;
    _isRun = false;
    _timerValue = 0;
    _currentTimer = 0;
    _isJustFinished = 0;
}

void Timer::setTimerInSecond(unsigned long time) {
    _timerValue = time * 1000;
}

void Timer::setTimerInMilli(unsigned long time) {
    _timerValue = time;
}

void Timer::update() {
    if(_isRun == true) {
        _currentTimer = (_timerValue - (millis() - _currentTime));
        if(_currentTimer <= 0) {
            _isRun = false;
            _currentTimer = 0;
            _isJustFinished = true;
        }
    } else {
        _isJustFinished = false;
    }
}

unsigned long Timer::getCurrentValueInSecond() {
    
    return _currentTimer / 1000;
}

unsigned long Timer::getCurrentValueInMilli() {
    
    return _currentTimer;
}

bool Timer::isFinished() {
    return !_isRun;
}

void Timer::start() {
    _isRun = true;
    _currentTime = millis();
}