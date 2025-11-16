#pragma once
#include "event.h"
#include <cstdint>


struct ProgressEvent : public Event {
  int8_t progressPercent;
};


