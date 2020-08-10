#include "ExternalEvent.h"

ExternalEvent::ExternalEvent()
{
  _state = false;
  _fired = false;
}

void ExternalEvent::fire() {
  _fired = true;
}

bool ExternalEvent::isFired() {
  return _state;
}

void ExternalEvent::update() {
  if(_fired) {
    _state = true;
    _fired = false;
  }
  else {
    _state = false;
  }
}

