#include "Arduino.h"
#include "PinOutput.h"

PinOutput::PinOutput()
{
  _pin = 0;
  _state = false;
}

void PinOutput::attach(uint8_t pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void PinOutput::toUp() {
  digitalWrite(_pin, true);
  _state = true;
}

void PinOutput::toDown() {
  digitalWrite(_pin, false);
  _state = false;
}

bool PinOutput::state() {
  return _state;
}

