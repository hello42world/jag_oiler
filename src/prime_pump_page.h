#pragma once
#include "page.h"
#include "ui/button_hint.h"

class PrimePumpPage : public Page {
  enum class State {
    Ready,
    Priming,
    Flushing
  };
public:
  PrimePumpPage(PageManager* pageManager);
  bool handleEvent(const Event* event) override;

private:
  void draw();

  void printInCenter(const char* str);

  State state_;
  ui::ButtonHint buttonHint_;
};
