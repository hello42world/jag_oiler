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

  constexpr static int16_t TURN_STEPS = 1106;

  Motor(EventBus* eventBus);

  void loop();
  bool handleEvent(const Event* event);

  void beginPortion(int32_t steps);
  int16_t getPortionProgress() const; // 0..100
  State getState() const;

private:
  AccelStepper stepper_;
  EventBus* eventBus_;
  State lastState_ = IDLE;
  int16_t lastProgress_ = 0;
};
