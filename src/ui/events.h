#pragma once
#include "event.h"
#include <cstdint>

namespace ui
{



struct FullRedrawEvent : public Event {
  FullRedrawEvent() : Event(EventID::FullRedraw) {}
};

struct ButtonEvent : public Event {
  ButtonEvent(int8_t btn) 
    : Event(EventID::Button)
    , button(btn) {}
  int8_t button;
};


} // namespace ui
