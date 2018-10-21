#ifndef PinOutput_h
#define PinOutput_h

#include "Arduino.h"

class PinOutput
{
  public:
    PinOutput();
    void attach(uint8_t pin);
    void toUp();
    void toDown();
    bool state();
  private:
    uint8_t _pin;
    bool _state;
};

#endif
