#pragma once
#include "page.h"

#include <MUIU8g2.h>

class MenuPage : public Page {
public:
  MenuPage(U8G2* u8g2);

  void loop(const Event* event) override;
private:
  void muiRedraw();

  MUIU8G2 mui_;
};
