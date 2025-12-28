#include "progress_bar.h"
#include "xmui.h"

namespace ui {

ProgressBar::ProgressBar(
  U8G2* u8g2, const std::string &label, int16_t x, int16_t y, int16_t w, int16_t h)
  : u8g2_(u8g2), label_(label), x_(x), y_(y), w_(w), h_(h), progress_(0) {
}

void ProgressBar::setProgress(int8_t percent) {
  auto oldProgress = progress_;
  if (percent < 0) {
    progress_ = 0;
  } else if (percent > 100) {
    progress_ = 100;
  } else {
    progress_ = percent;
  }
  if (oldProgress != progress_) {

    drawBar();
    xmuiUpdateDisplayArea(u8g2_, x_ + 2, y_ + 2, w_ - 4, h_ - 4);
  }
}

void ProgressBar::setLabel(const std::string &label) {
  if (label_ == label) {
    return;
  }
  label_ = label;
  drawLabel();
  xmuiUpdateDisplayArea(u8g2_, x_, y_ - 8, w_, 8);
}

int8_t ProgressBar::getProgress() const {
  return progress_;
}

void ProgressBar::draw() {
  drawLabel();

  u8g2_->drawFrame(x_, y_, w_, h_);

  drawBar();
}

void ProgressBar::drawBar() {
  int16_t filledWidth = (w_ - 4) * progress_ / 100;
  
  u8g2_->setDrawColor(1);
  u8g2_->drawBox(x_ + 2, y_ + 2, filledWidth, h_ - 4);
  u8g2_->setDrawColor(0);
  u8g2_->drawBox(x_ + 2 + filledWidth, y_ + 2, (w_ - 4) - filledWidth, h_ - 4);
  u8g2_->setDrawColor(1);
}

void ProgressBar::drawLabel() {
  u8g2_->setFont(u8g2_font_helvR08_tr);
  u8g2_->setDrawColor(0);
  u8g2_->drawBox(x_, y_ - 10, w_, 10);
  u8g2_->setDrawColor(1);
  u8g2_->drawStr(x_, y_ - 2, label_.c_str());
}

} // namespace ui
