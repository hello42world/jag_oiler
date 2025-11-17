#include "progress_bar.h"

namespace ui {

ProgressBar::ProgressBar(
  U8G2* u8g2, const std::string &label, int16_t x, int16_t y, int16_t w, int16_t h)
  : u8g2_(u8g2), label_(label), x_(x), y_(y), w_(w), h_(h), progress_(0) {
}

void updateDisplayArea(U8G2* u8g2, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    uint16_t sw = u8g2->getWidth();
    uint16_t sh = u8g2->getHeight();

    uint16_t x_end = x + w;
    uint16_t y_end = y + h;

    uint16_t x_prime = sw - x_end;
    uint16_t y_prime = sh - y_end;

    uint16_t x_prime_end = sw - x;
    uint16_t y_prime_end = sh - y;

    uint8_t x_tile = x_prime / 8;
    uint8_t y_tile = y_prime / 8;

    uint8_t x_tile_end = (x_prime_end + 7) / 8;
    uint8_t y_tile_end = (y_prime_end + 7) / 8;

    uint8_t w_tile = x_tile_end - x_tile;
    uint8_t h_tile = y_tile_end - y_tile;
    u8g2->updateDisplayArea(x_tile, y_tile, w_tile, h_tile);
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

    drawTheBar();
    updateDisplayArea(u8g2_, x_ + 2, y_ + 2, w_ - 4, h_ - 4);
  }
}

int8_t ProgressBar::getProgress() const {
  return progress_;
}

void ProgressBar::draw() {
  if (!label_.empty()) {
    u8g2_->setFont(u8g2_font_6x10_tf);
    u8g2_->drawStr(x_, y_ - 2, label_.c_str());
  }

  u8g2_->drawFrame(x_, y_, w_, h_);

  drawTheBar();
}

void ProgressBar::drawTheBar() {
  int16_t filledWidth = (w_ - 4) * progress_ / 100;
  

  u8g2_->setDrawColor(1);
  u8g2_->drawBox(x_ + 2, y_ + 2, filledWidth, h_ - 4);
  u8g2_->setDrawColor(0);
  u8g2_->drawBox(x_ + 2 + filledWidth, y_ + 2, (w_ - 4) - filledWidth, h_ - 4);
  u8g2_->setDrawColor(1);
}

} // namespace ui
