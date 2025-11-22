#include "dispense_controller.h"
#include "motor_events.h"
#include "menu_events.h"
#include "ui/events.h"
#include "U8g2lib.h"


DispenseController::DispenseController(EventBus* eventBus, Motor* motor, int8_t dropSize)
  : Controller(eventBus)
  , motor_(motor) 
  , dropSize_(dropSize)
{}

bool DispenseController::handleEvent(const Event* event) {
  if (event->id == EventID::Button 
      && static_cast<const ui::ButtonEvent*>(event)->button == U8X8_MSG_GPIO_MENU_HOME) {
      int32_t steps = dropSize_ * Motor::TURN_STEPS / 4;
      motor_->beginPortion(steps);
      Serial.printf("DispenseController: Dispensing %d drops (%d steps)\n", dropSize_, steps);
  } else if (event->id == EventID::SettingsChanged) {
    const SettingsChangedEvent* settingsEvent = static_cast<const SettingsChangedEvent*>(event);
    dropSize_ = settingsEvent->settings.dropSize;
    Serial.printf("DispenseController: Updated drop size to %d\n", dropSize_);
    return false; // propagate further
  } else {
    return false;
  }
  return true;
}
