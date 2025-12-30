#pragma once

#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <string>
#include "motor.h"

#include "page.h"
#include "settings.h"
#include "ui/battery_indicator.h"

class App : public PageManager {
  constexpr static int8_t NUM_PAGES = 3;
  constexpr static int8_t PAGE_DISPENSE = 0;
  constexpr static int8_t PAGE_MENU = 1;
  constexpr static int8_t PAGE_PRIME_PUMP = 2;
public: 
  App();

  void setup();
  void loop();

  // PageManager interface
  U8G2* u8g2() override;
  EventBus* eventBus() override;
  bool isActive(Page* page) override;

private:
  bool handleEvent(const Event* event);
  bool sendToMotor(const Event* event);
  void sendToPages(const Event* event);

  void activatePage(int8_t pageIndex);

  int8_t getButtonPress();
  void batteryDraw();
  void checkRedrawTimer();

  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_;
  EventBus eventBus_;
  Motor motor_;
  Settings settings_ = {2};

  Page* pages_[NUM_PAGES];

  Page* currentPage_;
  
  ui::BatteryIndicator batteryIndicator_;
  unsigned long lastRedrawTime_;
};