#include "motor.h" 
#include "motor_events.h"

#define PIN_AIN1 0
#define PIN_AIN2 1
#define PIN_BIN1 2
#define PIN_BIN2 3

// #define PIN_EN 10

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
  // stepper_.setEnablePin(PIN_EN);
}

void Motor::beginPortion(int32_t steps) {
  if (getState() == RUNNING_PORTION) {
    return;
  }
  stepper_.enableOutputs();
  stepper_.move(steps);
  eventBus_->publish(std::make_unique<MotorProgressEvent>(0, steps > 0));
  lastProgress_ = 0;
}

void Motor::loop() {
  stepper_.run();
  
  if (getState() != lastState_) {
    if (getState() == IDLE) {
      stepper_.disableOutputs();
      stepper_.setCurrentPosition(0);
      eventBus_->publish(std::make_unique<MotorProgressEvent>(100, false));
    }
  }

  if (getState() == RUNNING_PORTION) {
    auto progress = getPortionProgress();
    if (progress / progressStep != lastProgress_ / progressStep) {
      lastProgress_ = progress;
      eventBus_->publish(std::make_unique<MotorProgressEvent>(progress, stepper_.targetPosition() > 0));
    }
  }

  lastState_ = getState();
}

bool Motor::handleEvent(const Event* event) {
  if (event->id == EventID::MotorStartCommand) {
    const MotorStartCommandEvent* motorEvent = static_cast<const MotorStartCommandEvent*>(event);
    beginPortion(motorEvent->stepsToMove);
  } else if (event->id == EventID::MotorStopCommand) {
    stepper_.stop();
    eventBus_->publish(std::make_unique<MotorProgressEvent>(100, false));
  } else {
    return false;
  }
  return true;
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


