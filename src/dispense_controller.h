#pragma once
#include "controller.h"
#include "motor.h"

#include <memory>


class DispenseController : public Controller {
public:
  DispenseController(
    EventBus* eventBus, 
    Motor* motor, 
    int8_t dropSize);

  virtual bool handleEvent(const Event* event) override;
private:
  Motor*  motor_;
  int8_t dropSize_;
};
  