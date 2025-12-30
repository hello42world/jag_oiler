#pragma once

#include <U8g2lib.h>
#include <cstdint>


namespace ui {

class BatteryIndicator {
public:
  BatteryIndicator(U8G2* u8g2);

  void report(int32_t millivolts);
  void draw();

private:
  U8G2* u8g2_;
  int32_t millivolts_;
};

} // namespace ui
