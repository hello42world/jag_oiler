#include "battery_indicator.h"
#include <cstdio>


#define BATTERY_INDICATOR_WIDTH 26
#define BATTERY_INDICATOR_HEIGHT 11
#define BATTERY_EMPTY_MV 3000
#define BATTERY_FULL_MV 4050


namespace ui {

BatteryIndicator::BatteryIndicator(U8G2* u8g2)
  : u8g2_(u8g2), millivolts_(0) {
}

void BatteryIndicator::report(int32_t millivolts) {
  millivolts_ = millivolts;
}

void BatteryIndicator::draw() {
  constexpr int16_t w = BATTERY_INDICATOR_WIDTH;
  constexpr int16_t h = BATTERY_INDICATOR_HEIGHT;
  constexpr int16_t tipWidth = 2;
  constexpr int16_t tipHeight = 6;
  
  // Position at right top corner
  int16_t x = u8g2_->getDisplayWidth() - w;
  int16_t y = 0;
  
  // Draw main battery body (frame)
  u8g2_->drawFrame(x, y, w - tipWidth, h);
  
  // Draw battery tip (small rectangle on the right)
  int16_t tipY = y + (h - tipHeight) / 2;
  u8g2_->drawBox(x + w - tipWidth, tipY, tipWidth, tipHeight);
  
  // Calculate charge percentage from millivolts
  int32_t range = BATTERY_FULL_MV - BATTERY_EMPTY_MV;

  // Clamp voltage to valid range
  auto millivolts = millivolts_;
  if (millivolts < BATTERY_EMPTY_MV) {
    millivolts = BATTERY_EMPTY_MV;
  } else if (millivolts > BATTERY_FULL_MV) {
    millivolts = BATTERY_FULL_MV;
  }

  int8_t chargePercent = ((millivolts - BATTERY_EMPTY_MV) * 100) / range;
  
  // Draw charge percentage text in center
  char text[5];
  snprintf(text, sizeof(text), "%d%", chargePercent);
  
  u8g2_->setFont(u8g2_font_6x10_tf);
  int16_t textWidth = u8g2_->getStrWidth(text);
  int16_t textX = x + (w - tipWidth - textWidth) / 2;
  int16_t textY = y + h - 2;
  
  u8g2_->drawStr(textX, textY, text);

  sniprintf(text, sizeof(text), "%d", millivolts_);
  u8g2_->drawStr(0, textY, text);
}

} // namespace ui
