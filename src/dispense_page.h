#pragma once
#include "page.h"
#include "ui/progress_bar.h"

class DispensePage : public Page {
public:
  DispensePage(PageManager* pageManager, int8_t dropSize);

  bool handleEvent(const Event* event) override;
private:
  void drawInfo();

  ui::ProgressBar progressBar_;
  int8_t dropSize_;
};
