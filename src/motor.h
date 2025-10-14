#pragma once

#include <Arduino.h>


class Motor {
public:
  enum State {
    IDLE,
    RUNNING_PORTION
  };

  void setup();
  void loop();
  void beginPortion(int steps);
  int getPortionProgress() const; // 0..100
  State getState() const;
private:
  void advance();

  State state_ = IDLE;
  int stepsTotal_ = 0;
  int stepsRemaining_ = 0;
};
