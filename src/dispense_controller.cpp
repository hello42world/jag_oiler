#include "dispense_controller.h"
#include "dispense_events.h"
#include "ui/events.h"
#include "U8g2lib.h"


DispenseController::DispenseController(EventBus* eventBus, Motor* motor)
  : Controller(eventBus)
  , motor_(motor) 
{}

const int TURN = 1108; 

bool DispenseController::handleEvent(const Event* event) {
  if (event->id == EventID::Button) {
    const ui::ButtonEvent* buttonEvent = static_cast<const ui::ButtonEvent*>(event);
    if (buttonEvent->button == U8X8_MSG_GPIO_MENU_SELECT) {
      motor_->beginPortion(TURN / 2);
    }
    return true;
  }
  return false;
}
