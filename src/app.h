#pragma once

#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <string>
#include "motor.h"

#include "dispense_page.h"
#include "dispense_controller.h"
#include "menu_page.h"


class App {
  constexpr static int8_t NUM_PAGES = 2;
  constexpr static int8_t PAGE_DISPENSE = 0;
  constexpr static int8_t PAGE_MENU = 1;
public: 
  App();
  ~App();
  void setup();
  void loop();


private:
  bool handleControllers(const Event* event);
  bool handleEvent(const Event* event);
  void activatePage(int8_t pageIndex);

  int8_t getButtonPress();
  int8_t getButtonPress2();


  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_;
  EventBus eventBus_;
  Motor motor_;
  Settings settings_ = {2};

  Page* pages_[NUM_PAGES];
  Controller* controllers_[NUM_PAGES];

  Page* currentPage_;
  Controller* currentController_;
};