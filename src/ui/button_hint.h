#pragma once

#include <U8g2lib.h>

#include <string>


namespace ui {

class ButtonHint {
public:
  ButtonHint(U8G2* u8g2, const std::string& btn1, const std::string& btn2,
             const std::string& btn3);

  void draw();
private:

  U8G2* u8g2_;
  std::string btn1_;
  std::string btn2_;
  std::string btn3_;
};

}
