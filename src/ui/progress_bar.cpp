#include "progress_bar.h"

namespace ui {

ProgressBar::ProgressBar(const std::string &label, int16_t x, int16_t y, int16_t w, int16_t h)
  : label_(label), x_(x), y_(y), w_(w), h_(h), progress_(0) {
}

void ProgressBar::setProgress(int8_t percent) {
  if (percent < 0) {
    progress_ = 0;
  } else if (percent > 100) {
    progress_ = 100;
  } else {
    progress_ = percent;
  }
}

int8_t ProgressBar::getProgress() const {
  return progress_;
}

void ProgressBar::draw(U8G2 &u8g2) {
  if (!label_.empty()) {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(x_, y_ - 2, label_.c_str());
  }

  u8g2.drawFrame(x_, y_, w_, h_);
  
  int16_t filledWidth = (w_ - 4) * progress_ / 100;
  
  if (filledWidth > 0) {
    u8g2.drawBox(x_ + 2, y_ + 2, filledWidth, h_ - 4);
  }
}

void ProgressBar::updateDisplay(U8G2 &u8g2) {
  u8g2.updateDisplayArea(x_ + 2, y_ + 2, w_ - 4, h_ - 4);
}

} // namespace ui
