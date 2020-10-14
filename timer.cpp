#include "timer.h"
#include "Arduino.h"



/**
 * Constructor
 */
Timer::Timer()
{
    _currentTime = 0;
    _run = false;
    _timerValue = 0;
    _currentTimer = 0;
    _justFinished = false;
}

void Timer::setTimerInSecond(unsigned long time) {
    _timerValue = time * 1000;
}

void Timer::setTimerInMilli(unsigned long time) {
    _timerValue = time;
}

void Timer::update() {
    if(_run == true) {
        _currentTimer = (_timerValue - (millis() - _currentTime));
        if(_currentTimer <= 0) {
            _run = false;
            _currentTimer = 0;
            _justFinished = true;
        }
    } else {
        _justFinished = false;
    }
}

unsigned long Timer::getCurrentValueInSecond() {
    
    return _currentTimer / 1000;
}

unsigned long Timer::getCurrentValueInMilli() {
    
    return _currentTimer;
}

bool Timer::isFinished() {
    return !_run;
}

bool Timer::isRun() {
    return _run;
}

bool Timer::isJustFinished() {
  return _justFinished;
}

void Timer::start() {
    _run = true;
    _justFinished = false;
    _currentTime = millis();
}
