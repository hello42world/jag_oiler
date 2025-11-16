#pragma once
#include "event.h"
#include "motor.h"

#include <memory>

class Controller {
public:
  Controller(EventBus* eventBus);

  virtual void loop(const Event* event) = 0;

protected:
  void publishEvent(std::unique_ptr<Event> event);
private:
  EventBus* eventBus_;
};

//
//
//
class DispenseController : public Controller {
public:
  DispenseController(EventBus* eventBus, Motor* motor);

  virtual void loop(const Event* event) override;
private:
  Motor*  motor_;
};
