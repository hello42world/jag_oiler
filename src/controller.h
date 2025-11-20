#pragma once
#include "event.h"


class Controller {
public:
  Controller(EventBus* eventBus);

  virtual void loop() {};
  virtual bool handleEvent(const Event* event) { return false; }

protected:
  void publishEvent(std::unique_ptr<Event> event);
private:
  EventBus* eventBus_;
};