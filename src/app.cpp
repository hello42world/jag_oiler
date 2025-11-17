#include "app.h"
#include "ui/consts.h"
#include "ui/events.h"

constexpr uint8_t PIN_BTN_UP = 5;
constexpr uint8_t PIN_BTN_SEL = 7;
constexpr uint8_t PIN_BTN_DOWN = 6;

//
// App
//
App::App() 
  : u8g2_{U8G2_R2, U8X8_PIN_NONE, 9, 8}
  , eventBus_{}
  , motor_{&eventBus_}
  , dispensePage_(&u8g2_)
  , dispenseController_(&eventBus_, &motor_)
  , currentPage_(&dispensePage_)
  , currentController_(&dispenseController_)
{
}

void App::setup() {
  pinMode(PIN_BTN_UP, INPUT_PULLUP);
  pinMode(PIN_BTN_SEL, INPUT_PULLUP);
  pinMode(PIN_BTN_DOWN, INPUT_PULLUP);

  u8g2_.begin(/* menu_select_pin= */ PIN_BTN_SEL, 
    /* menu_next_pin= */ U8X8_PIN_NONE, /* menu_prev_pin= */ U8X8_PIN_NONE, 
    /* menu_up_pin= */ PIN_BTN_UP, /* menu_down_pin= */PIN_BTN_DOWN, 
    /* menu_home_pin= */ U8X8_PIN_NONE);  
  
    // mui.begin(u8g2, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));
  // mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);

  eventBus_.publish(std::make_unique<ui::FullRedrawEvent>());
}

void App::loop() {
  int8_t btn = getButtonPress();
  if (btn != 0) {
    eventBus_.publish(std::make_unique<ui::ButtonEvent>(btn));
  }

  motor_.loop();

  for (;;) {  
    auto currentEvent = eventBus_.consume();
    currentController_->loop(currentEvent.get());

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
