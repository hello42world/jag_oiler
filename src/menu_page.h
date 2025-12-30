#pragma once
#include "page.h"
#include "ui/xmui.h"
#include "settings.h"

class MenuPage : public Page {
public:
  constexpr static int8_t RES_DONE = 0;
  constexpr static int8_t RES_PRIME_PUMP = 1;

  MenuPage(PageManager* pageManager, const Settings& settings);

  bool handleEvent(const Event* event) override;
private:
  void draw();
  void handleBtnHome();
  void handleBtnSelect();

  MUIU8G2 mui_;
  std::vector<muif_struct> muifList_;

  Settings settings_;

  mui_u8g2_s8_min_max_struct stDropSize_;
};
