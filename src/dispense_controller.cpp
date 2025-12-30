#include "dispense_controller.h"
#include "motor_events.h"
#include "menu_events.h"
#include "ui/events.h"
#include "U8g2lib.h"

DispenseController::DispenseController(EventBus* eventBus, int8_t dropSize)
  : Controller(eventBus)
  , dropSize_(dropSize)
{}

bool DispenseController::handleEvent(const Event* event) {
  if (event->id == EventID::SettingsChanged) {
    const SettingsChangedEvent* settingsEvent = static_cast<const SettingsChangedEvent*>(event);
    dropSize_ = settingsEvent->settings.dropSize;
    Serial.printf("DispenseController: Updated drop size to %d\n", dropSize_);
  }
  return false; // propagate further  
}
