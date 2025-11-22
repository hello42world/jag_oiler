#include "event.h"

struct MotorProgressEvent : public Event {
  MotorProgressEvent(int8_t percent, bool forward)
    : Event(EventID::MotorProgress)
    , progressPercent(percent)
    , forward(forward) {}
  int8_t progressPercent;
  bool forward;
};


struct MotorStartCommandEvent : public Event {
  MotorStartCommandEvent(int32_t steps) 
    : Event(EventID::MotorStartCommand)
    , stepsToMove(steps) {}
  int32_t stepsToMove;
};

struct MotorStopCommandEvent : public Event {
  MotorStopCommandEvent() 
    : Event(EventID::MotorStopCommand) {}
};

