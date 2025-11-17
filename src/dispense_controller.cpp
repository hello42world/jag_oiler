#include "dispense_controller.h"
#include "ui/events.h"
#include "U8g2lib.h"

Controller::Controller(EventBus* eventBus) 
  : eventBus_(eventBus) 
{}

void Controller::publishEvent(std::unique_ptr<Event> event) {
  eventBus_->publish(std::move(event));
}

//
//
//
DispenseController::DispenseController(EventBus* eventBus, Motor* motor)
  : Controller(eventBus)
  , motor_(motor) 
{}

void DispenseController::loop(const Event* event) {
  if (event->id == EventID::Button) {
    const ui::ButtonEvent* buttonEvent = static_cast<const ui::ButtonEvent*>(event);
    if (buttonEvent->button == U8X8_MSG_GPIO_MENU_SELECT) {
      Serial.println("Select pressed");
    }
  }
}
