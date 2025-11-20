#pragma once
#include "controller.h"
#include "motor.h"

#include <memory>


class DispenseController : public Controller {
public:
  DispenseController(EventBus* eventBus, Motor* motor);

  virtual bool handleEvent(const Event* event) override;
private:
  Motor*  motor_;
};
