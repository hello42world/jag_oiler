#include "controller.h"


Controller::Controller(EventBus* eventBus) 
  : eventBus_(eventBus) 
{}

void Controller::publishEvent(std::unique_ptr<Event> event) {
  eventBus_->publish(std::move(event));
}