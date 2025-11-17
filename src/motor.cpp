#include "motor.h" 
#include "dispense_events.h"

#define PIN_AIN1 0
#define PIN_AIN2 1
#define PIN_BIN1 2
#define PIN_BIN2 3

#define PIN_EN 10

const int progressStep = 10;

Motor::Motor(EventBus* eventBus) 
  : stepper_(AccelStepper::FULL4WIRE, PIN_AIN1, PIN_AIN2, PIN_BIN1, PIN_BIN2) 
  , eventBus_(eventBus)
  , lastState_(IDLE)
{
  
  stepper_.setMaxSpeed(1000);    
  stepper_.setAcceleration(15000);  
  stepper_.setSpeed(300);
  stepper_.disableOutputs();
  stepper_.setEnablePin(PIN_EN);
}

void Motor::beginPortion(int steps) {
  if (getState() == RUNNING_PORTION) {
    return;
  }
  stepper_.enableOutputs();
  stepper_.move(steps);
  eventBus_->publish(std::make_unique<PortionProgressEvent>(0));
  lastProgress_ = 0;
}

void Motor::loop() {
  stepper_.run();
  
  if (getState() != lastState_) {
    if (getState() == IDLE) {
      stepper_.disableOutputs();
      stepper_.setCurrentPosition(0);
      eventBus_->publish(std::make_unique<PortionProgressEvent>(0));
    }
  }

  if (getState() == RUNNING_PORTION) {
    auto progress = getPortionProgress();
    if (progress / progressStep != lastProgress_ / progressStep) {
      lastProgress_ = progress;
      eventBus_->publish(std::make_unique<PortionProgressEvent>(progress));
    }
  }

  lastState_ = getState();
}

int16_t Motor::getPortionProgress() const {
  if (getState() == IDLE) {
    return 100;
  }
  AccelStepper& stepper = const_cast<AccelStepper&>(stepper_);
  int16_t totalSteps = abs(stepper.targetPosition());
  int16_t remainingSteps = abs(stepper.distanceToGo());
  return (totalSteps - remainingSteps) * 100 / totalSteps;
}

Motor::State Motor::getState() const {
  return const_cast<AccelStepper&>(stepper_).isRunning() ? RUNNING_PORTION : IDLE;
}


