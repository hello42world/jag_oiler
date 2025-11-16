#pragma once
#include "event.h"
#include <cstdint>

namespace ui
{

struct FullRedrawEvent : public Event {
};

struct ButtonEvent : public Event {
  ButtonEvent(int8_t pin) : buttonPin(pin) {}
  int8_t buttonPin;
};



} // namespace ui
