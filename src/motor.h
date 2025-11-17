#pragma once

#include <Arduino.h>
#include <AccelStepper.h>

#include "event.h"


class Motor {
public:
  enum State {
    IDLE,
    RUNNING_PORTION
  };

  Motor(EventBus* eventBus);

  void loop();
  void beginPortion(int steps);
  int16_t getPortionProgress() const; // 0..100
  State getState() const;

private:
  AccelStepper stepper_;
  EventBus* eventBus_;
  State lastState_ = IDLE;
  int16_t lastProgress_ = 0;
};
