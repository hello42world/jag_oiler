#pragma once

#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <string>
#include "motor.h"

#include "dispense_page.h"
#include "dispense_controller.h"
#include "menu_page.h"


class App {
public: 
  App();
  void setup();
  void loop();


private:
  bool handleEvent(const Event* event);
  void activatePage(Page* page, Controller* controller);

  int8_t getButtonPress();
  int8_t getButtonPress2();


  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_;
  EventBus eventBus_;
  Motor motor_;


  //MUIU8G2 mui;
  Settings settings_ = {2};

  DispensePage dispensePage_;
  DispenseController dispenseController_;
  MenuPage menuPage_;

  Page* currentPage_;
  Controller* currentController_;
};