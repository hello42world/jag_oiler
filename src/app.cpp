#include "app.h"
#include "ui/consts.h"
#include "ui/events.h"

#include "dispense_page.h"
#include "dispense_controller.h"
#include "menu_page.h"
#include "menu_events.h"
#include "prime_pump_page.h"
#include "battery.h"


constexpr uint8_t PIN_BTN_SEL  = 3;
constexpr uint8_t PIN_BTN_PREV = 4;
constexpr uint8_t PIN_BTN_NEXT = 2;
constexpr uint8_t PIN_BTN_HOME = 1;

//
// App
//
App::App() 
  : u8g2_{U8G2_R0, U8X8_PIN_NONE, 9, 8}
  , eventBus_{}
  , motor_{&eventBus_}
  , currentPage_{nullptr}
  , currentController_{nullptr}
  , batteryIndicator_{&u8g2_}
  , lastRedrawTime_{0}
{ 
  // Load settings here

  // Initialize pages
  pages_[PAGE_DISPENSE] = new DispensePage(&u8g2_, &eventBus_, settings_.dropSize);
  pages_[PAGE_MENU] = new MenuPage(&u8g2_, &eventBus_, settings_);
  pages_[PAGE_PRIME_PUMP] = new PrimePumpPage(&u8g2_, &eventBus_);

  
  // Initialize controllers
  controllers_[PAGE_DISPENSE] = new DispenseController(&eventBus_, settings_.dropSize);
  controllers_[PAGE_MENU] = nullptr;
  controllers_[PAGE_PRIME_PUMP] = nullptr;
  // Set initial page
  currentPage_ = pages_[PAGE_DISPENSE];
  currentController_ = controllers_[PAGE_DISPENSE];
}

void App::setup() {
  pinMode(PIN_BTN_PREV, INPUT_PULLUP);
  pinMode(PIN_BTN_SEL, INPUT_PULLUP);
  pinMode(PIN_BTN_NEXT, INPUT_PULLUP);
  pinMode(PIN_BTN_HOME, INPUT_PULLUP);

  // We don't use the default u8g2 key handling because it fires when the button is _released_.
  // getButtonPress fires on press.
  u8g2_.begin(
    /* menu_select_pin = */ U8X8_PIN_NONE,
    /* menu_next_pin   = */ U8X8_PIN_NONE,
    /* menu_prev_pin   = */ U8X8_PIN_NONE,
    /* menu_up_pin     = */ U8X8_PIN_NONE,
    /* menu_down_pin   = */ U8X8_PIN_NONE,
    /* menu_home_pin   = */ U8X8_PIN_NONE
  );

  batteryInit();

  eventBus_.publish(std::make_unique<ui::FullRedrawEvent>());
}

void App::loop() {
  checkRedrawTimer();

  int8_t btn = getButtonPress();
  if (btn != 0) {
    eventBus_.publish(std::make_unique<ui::ButtonEvent>(btn));
  }

  motor_.loop();

  for (;;) {  
    auto currentEvent = eventBus_.consume();
    if (!currentEvent) {
      break;
    }

    if (handleEvent(currentEvent.get())) {
      continue;
    }

    if (sendToMotor(currentEvent.get())) {
      continue;
    }

    if (sendToControllers(currentEvent.get())) {
      continue;
    }

    sendToPages(currentEvent.get());
  } 
}


bool App::handleEvent(const Event* event) {
  if (event->id == EventID::PageClosed) {
    PageClosedEvent const* pageClosedEvent = static_cast<const PageClosedEvent*>(event);
    if (pageClosedEvent->page == pages_[PAGE_DISPENSE]) {
      activatePage(PAGE_MENU);
    } else if (pageClosedEvent->page == pages_[PAGE_MENU]) {
      if (pageClosedEvent->result == MenuPage::RES_DONE) { 
        activatePage(PAGE_DISPENSE);
      } else if (pageClosedEvent->result == MenuPage::RES_PRIME_PUMP) {
        activatePage(PAGE_PRIME_PUMP);
      }
    } else if (pageClosedEvent->page == pages_[PAGE_PRIME_PUMP]) {
      activatePage(PAGE_MENU);
    }
    return true;
  } else if (event->id == EventID::SettingsChanged) {
    const SettingsChangedEvent* settingsEvent = static_cast<const SettingsChangedEvent*>(event);
    settings_ = settingsEvent->settings;
  }

  return false; // propagate further
}

bool App::sendToControllers(const Event* event) {
  if (event->id == EventID::Button) {
    // Button event is only sent to the current controller
    if (currentController_) {
      return currentController_->handleEvent(event);
    }
  } else {
    for (int8_t i = 0; i < NUM_PAGES; i++) {
      if (controllers_[i] && controllers_[i]->handleEvent(event)) {
        return true;
      }
    }
  }
  return false; 
}

bool App::sendToMotor(const Event* event) {
  if (event->id >= EventID::MotorStartCommand && event->id <= EventID::MotorStopCommand) {
    return motor_.handleEvent(event);
  }
  return false;
}

void App::sendToPages(const Event* event) 
{
  if (event->id == EventID::FullRedraw) {
    u8g2_.clearBuffer();
    currentPage_->handleEvent(event);
    batteryDraw();
    u8g2_.sendBuffer();
  } else {  
    currentPage_->handleEvent(event);
  }
}

void App::activatePage(int8_t pageIndex) {
  currentPage_ = pages_[pageIndex];
  currentController_ = controllers_[pageIndex];
  eventBus_.publish(std::make_unique<PageActivatedEvent>());
  eventBus_.publish(std::make_unique<ui::FullRedrawEvent>());
}

int8_t App::getButtonPress() {
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

void App::batteryDraw() {
  int32_t voltage_mv = batteryReadVoltage();
  if (voltage_mv > 0) {
    Serial.printf("Battery voltage: %d mV\n", voltage_mv);
    batteryIndicator_.report(voltage_mv);
    batteryIndicator_.draw();
  }
}

void App::checkRedrawTimer() {
  unsigned long currentTime = millis();
  if (currentTime - lastRedrawTime_ >= 60 * 1000) { 
    lastRedrawTime_ = currentTime;
    eventBus_.publish(std::make_unique<ui::FullRedrawEvent>());
  }
}


