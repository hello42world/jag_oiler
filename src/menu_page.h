#pragma once
#include "page.h"
#include "ui/xmui.h"
#include "settings.h"

class MenuPage : public Page {
public:
  MenuPage(U8G2* u8g2, const Settings& settings);

  bool handleEvent(const Event* event) override;
private:
  void muiRedraw();

  MUIU8G2 mui_;
  std::vector<muif_struct> muifList_;

  Settings settings_;

  mui_u8g2_s8_min_max_struct stDropSize_;
};
