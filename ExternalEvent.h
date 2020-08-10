#ifndef ExternalEvent_h
#define ExternalEvent_h

class ExternalEvent
{
  public:
    ExternalEvent();
    void fire();
    bool isFired();
    void update();
  private:
    bool _state;
    bool _fired;
};

#endif
