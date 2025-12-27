#pragma once
#include "controller.h"
#include "motor.h"

#include <memory>


class DispenseController : public Controller {
public:
  DispenseController(
    EventBus* eventBus, 
    int8_t dropSize);

  virtual bool handleEvent(const Event* event) override;
private:
  int8_t dropSize_;
};
  