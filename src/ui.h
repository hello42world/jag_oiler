#pragma once

#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <string>

//  
// ProgressBar
//
class ProgressBar {
public:
  ProgressBar(const std::string &label, int x, int y, int w, int h);
  void setProgress(int percent);
  int getProgress() const;
  void draw(U8G2 &u8g2);

private:
  std::string label_;
  int x_, y_, w_, h_;
  int progress_;
};


//
// UI
//
class UI {
  const int PIN_BTN_UP = 1;  
  const int PIN_BTN_SEL = 2;  
  const int PIN_BTN_DOWN = 3;  
public: 
  UI();
  void setup();
  void loop();
  int8_t getButtonEvent();
  
  void setProgress(int percent);

private:


  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_;
  //MUIU8G2 mui;
  ProgressBar progressBar_{"Progress", 10, 30, 108, 20};
  int testProgress_ = 0;
  bool needsRedraw_ = true;
};