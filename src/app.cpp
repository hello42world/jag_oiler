#include "app.h"
#include "ui/consts.h"
#include "ui/events.h"


constexpr uint8_t PIN_BTN_SEL = 5;
constexpr uint8_t PIN_BTN_PREV = 10;
constexpr uint8_t PIN_BTN_NEXT = 6;
constexpr uint8_t PIN_BTN_HOME = 7;

//
// App
//
App::App() 
  : u8g2_{U8G2_R2, U8X8_PIN_NONE, 9, 8}
  , eventBus_{}
  , motor_{&eventBus_}
  , dispensePage_(&u8g2_)
  , dispenseController_(&eventBus_, &motor_)
  , menuPage_(&u8g2_)
  //, currentPage_(&dispensePage_)
  //, currentController_(&dispenseController_)
  , currentPage_(&menuPage_)
  , currentController_(nullptr)
{
}

void App::setup() {
  pinMode(PIN_BTN_PREV, INPUT_PULLUP);
  pinMode(PIN_BTN_SEL, INPUT_PULLUP);
  pinMode(PIN_BTN_NEXT, INPUT_PULLUP);
  pinMode(PIN_BTN_HOME, INPUT_PULLUP);
# if 0
  u8g2_.begin(/* menu_select_pin= */ PIN_BTN_SEL, 
    /* menu_next_pin= */ PIN_BTN_NEXT, /* menu_prev_pin= */ PIN_BTN_PREV, 
    /* menu_up_pin= */ U8X8_PIN_NONE, /* menu_down_pin= */U8X8_PIN_NONE, 
    /* menu_home_pin= */ PIN_BTN_HOME);     
# endif
    u8g2_.begin(
      /* menu_select_pin = */ U8X8_PIN_NONE,
      /* menu_next_pin   = */ U8X8_PIN_NONE,
      /* menu_prev_pin   = */ U8X8_PIN_NONE,
      /* menu_up_pin     = */ U8X8_PIN_NONE,
      /* menu_down_pin   = */ U8X8_PIN_NONE,
      /* menu_home_pin   = */ U8X8_PIN_NONE
    );

  eventBus_.publish(std::make_unique<ui::FullRedrawEvent>());
}

void App::loop() {
  int8_t btn = getButtonPress2();
  if (btn != 0) {
    eventBus_.publish(std::make_unique<ui::ButtonEvent>(btn));
    // Serial.printf("Button pressed: %d\n", btn);
  }

  motor_.loop();

  for (;;) {  
    auto currentEvent = eventBus_.consume();
    if (currentController_) {
      currentController_->loop(currentEvent.get());
    }

    if (!currentEvent) {
      // Page has nothing to do if no event
      break;
    }

    if (currentEvent->id == EventID::FullRedraw) {
      u8g2_.clearBuffer();
    }
    currentPage_->loop(currentEvent.get());
    if (currentEvent->id == EventID::FullRedraw) {
      u8g2_.sendBuffer();
    }
  } 

}

int8_t App::getButtonPress() {
  return u8g2_.getMenuEvent();
}

int8_t App::getButtonPress2() {
  static unsigned long lastDebounceTime = 0;
  const uint8_t pins[] = {PIN_BTN_SEL, PIN_BTN_PREV, PIN_BTN_NEXT, PIN_BTN_HOME};
  static constexpr uint8_t numbtns = sizeof(pins) / sizeof(pins[0]);

  static bool lastButtonState[numbtns] = {HIGH}; // Previous button states
  static bool buttonState[numbtns] = {HIGH};     // Current stable states
  static const unsigned long debounceDelay = 50; // 50ms debounce time

  const int8_t buttonValues[numbtns] = {
     U8X8_MSG_GPIO_MENU_SELECT, 
     U8X8_MSG_GPIO_MENU_PREV, 
     U8X8_MSG_GPIO_MENU_NEXT, 
     U8X8_MSG_GPIO_MENU_HOME}; // Return values for each button

  unsigned long currentTime = millis();
  
  // Read current button states
  bool reading[numbtns];
  for (int8_t i = 0; i < numbtns; i++) {
    reading[i] = digitalRead(pins[i]);
  }
  
  // Check if any button state has changed
  bool stateChanged = false;
  for (int8_t i = 0; i < numbtns; i++) {
    if (reading[i] != lastButtonState[i]) {
      stateChanged = true;
      break;
    }
  }
  
  // If state changed, reset debounce timer
  if (stateChanged) {
    lastDebounceTime = currentTime;
  }
  
  // Update last button states
  for (int8_t i = 0; i < numbtns; i++) {
    lastButtonState[i] = reading[i];
  }
  
  // Check if enough time has passed for debouncing
  if ((currentTime - lastDebounceTime) > debounceDelay) {
    // Check each button for press (HIGH to LOW transition)
    for (int8_t i = 0; i < numbtns; i++) {
      if (buttonState[i] == HIGH && reading[i] == LOW) {
        buttonState[i] = LOW;
        return buttonValues[i]; // Return button ID
      }
      if (buttonState[i] == LOW && reading[i] == HIGH) {
        buttonState[i] = HIGH;
      }
    }
  }
  
  return 0; // No button pressed
}


