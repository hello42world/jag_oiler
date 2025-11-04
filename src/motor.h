#pragma once

#include <Arduino.h>
#include <AccelStepper.h>


class Motor {
public:
  enum State {
    IDLE,
    RUNNING_PORTION
  };


  Motor();

  // void setup();
  void loop();
  void beginPortion(int steps);
  int getPortionProgress() const; // 0..100
  State getState() const;

private:
  AccelStepper stepper_;

  State lastState_ = IDLE;
};
