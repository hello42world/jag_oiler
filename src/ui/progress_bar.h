#pragma once

#include <U8g2lib.h>
#include <string>
#include <cstdint>


namespace ui {

class ProgressBar {
public:
  ProgressBar(U8G2* u8g2, const std::string &label, int16_t x, int16_t y, int16_t w, int16_t h);
  void setProgress(int8_t percent);
  void setLabel(const std::string &label);
  int8_t getProgress() const;
  void draw();

private:
  void drawBar();
  void drawLabel();

  U8G2* u8g2_;
  std::string label_;
  int16_t x_, y_, w_, h_;
  int8_t progress_;
};

}
