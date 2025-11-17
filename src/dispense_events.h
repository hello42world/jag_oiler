#pragma once
#include "event.h"
#include <cstdint>


struct PortionProgressEvent : public Event {
  PortionProgressEvent(int8_t percent) 
    : Event(EventID::PortionProgress)
    , progressPercent(percent) {}
  int8_t progressPercent;
};


 