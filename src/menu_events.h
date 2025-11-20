#pragma once

#include "event.h"
#include "settings.h"


struct SettingsChangedEvent : public Event {
  SettingsChangedEvent(const Settings& _settings)
    : Event(EventID::SettingsChanged)
    , settings(_settings) {}
  Settings settings;
};


 