#pragma once
#include "page.h"
#include "ui/progress_bar.h"

class DispensePage : public Page {
public:
  DispensePage(U8G2* u8g2);

  void loop(const Event* event) override;
private:
  ui::ProgressBar progressBar_{"Dispensing...", 10, 30, 108, 20};
};
