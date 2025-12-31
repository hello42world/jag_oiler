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
  int32_t dropsDispensed_ = 0;
  int32_t dropUnitsDispensed_ = 0;
};
