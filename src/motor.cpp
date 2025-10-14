#include "motor.h" 

const int PIN_STEP = 0;
const int PIN_ENABLE = 4;
const int PIN_DIR = 10;
const int STEP_DELAY_US = 35;

void Motor::setup() {
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  digitalWrite(PIN_ENABLE, HIGH);
}

void Motor::beginPortion(int steps) {
  stepsTotal_ = steps;
  stepsRemaining_ = steps;
  state_ = RUNNING_PORTION;
  digitalWrite(PIN_ENABLE, LOW);
  digitalWrite(PIN_DIR, LOW);
}

void Motor::loop() {
  switch(state_) {
    case IDLE:
      break;
    case RUNNING_PORTION:
      if (stepsRemaining_ > 0) {
        advance();
      } else {
        state_ = IDLE;
        digitalWrite(PIN_ENABLE, HIGH);
      }
      break;
  }
}

int Motor::getPortionProgress() const {
  if (state_ == RUNNING_PORTION) {
    return 100 - (stepsRemaining_ * 100 / stepsTotal_);
  }
  return 0;
}

Motor::State Motor::getState() const {
  return state_;
}

void Motor::advance() {
  int numStepsPerAdvance = stepsTotal_ / 10;
  stepsRemaining_ -= numStepsPerAdvance;
  
  if (stepsRemaining_ < numStepsPerAdvance) {
    numStepsPerAdvance += stepsRemaining_;
    stepsRemaining_ = 0;
  }

  for (int i = 0; i < numStepsPerAdvance; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(STEP_DELAY_US);
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(STEP_DELAY_US);
  } 

}

