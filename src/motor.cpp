#include "motor.h" 

#define PIN_AIN1 0
#define PIN_AIN2 1
#define PIN_BIN1 2
#define PIN_BIN2 3

#define PIN_EN 10

Motor::Motor() 
  : stepper_(AccelStepper::FULL4WIRE, PIN_AIN1, PIN_AIN2, PIN_BIN1, PIN_BIN2) 
  , lastState_(IDLE)
{
  
  stepper_.setMaxSpeed(1000);    
  stepper_.setAcceleration(5000);  
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
}

void Motor::loop() {
  stepper_.run();
  
  if (getState() != lastState_) {
    if (getState() == IDLE) {
      Serial.println("Portion completed");
      stepper_.disableOutputs();
      stepper_.setCurrentPosition(0);
    }
  }

  lastState_ = getState();
}

int Motor::getPortionProgress() const {
  if (getState() == IDLE) {
    return 100;
  }
  AccelStepper& stepper = const_cast<AccelStepper&>(stepper_);
  int totalSteps = abs(stepper.targetPosition());
  int remainingSteps = abs(stepper.distanceToGo());
  auto percent = (totalSteps - remainingSteps) * 100 / totalSteps;
  return percent;
}

Motor::State Motor::getState() const {
  return const_cast<AccelStepper&>(stepper_).isRunning() ? RUNNING_PORTION : IDLE;
}


