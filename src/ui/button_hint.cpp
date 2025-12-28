#include "button_hint.h"
#include "ui/xmui.h"

namespace ui {

ButtonHint::ButtonHint(U8G2* u8g2, const std::string& btn1, const std::string& btn2,
                       const std::string& btn3)
  : u8g2_(u8g2), btn1_(btn1), btn2_(btn2), btn3_(btn3) {
}

void ButtonHint::draw() {
  constexpr int16_t yPos = 56;
  constexpr int16_t height = 8;
  constexpr int16_t d = 7;
  int16_t screenWidth = u8g2_->getDisplayWidth();
  int16_t cellWidth = screenWidth / 3;

  u8g2_->setFont(XMUI_DEFAULT_FONT);
  // Draw button 1
  int16_t textW1 = u8g2_->getStrWidth(btn1_.c_str());
  int16_t textX1 = (cellWidth - textW1) / 2;
  u8g2_->drawStr(textX1, yPos + height - 1, btn1_.c_str());

  // Draw vertical divider after button 1
  u8g2_->drawVLine(cellWidth, yPos - d, height + d);

  // Draw button 2
  int16_t textW2 = u8g2_->getStrWidth(btn2_.c_str());
  int16_t textX2 = cellWidth + (cellWidth - textW2) / 2;
  u8g2_->drawStr(textX2, yPos + height - 1, btn2_.c_str());

  // Draw vertical divider after button 2
  u8g2_->drawVLine(cellWidth * 2, yPos - d, height + d);

  // Draw button 3
  int16_t textW3 = u8g2_->getStrWidth(btn3_.c_str());
  int16_t textX3 = cellWidth * 2 + (cellWidth - textW3) / 2;
  u8g2_->drawStr(textX3, yPos + height - 1, btn3_.c_str());

  // Draw top horizontal line
  u8g2_->drawHLine(0, yPos - d, screenWidth);
}

} // namespace ui