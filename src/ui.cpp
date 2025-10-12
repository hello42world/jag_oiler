#include "ui.h"

//  
// ProgressBar
//
ProgressBar::ProgressBar(const std::string &label, int x, int y, int w, int h)
  : label_(label), x_(x), y_(y), w_(w), h_(h), progress_(0) {
}

void ProgressBar::setProgress(int percent) {
  if (percent < 0) {
    progress_ = 0;
  } else if (percent > 100) {
    progress_ = 100;
  } else {
    progress_ = percent;
  }
}

void ProgressBar::draw(U8G2 &u8g2) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(x_, y_ - 2, label_.c_str());
  
  u8g2.drawFrame(x_, y_, w_, h_);
  
  int filledWidth = (w_ - 4) * progress_ / 100;
  
  if (filledWidth > 0) {
    u8g2.drawBox(x_ + 2, y_ + 2, filledWidth, h_ - 4);
  }
}

//
// UI
//
UI::UI() 
: u8g2_(U8G2_R2, U8X8_PIN_NONE, 9, 8)
//, mui()
{
}

void UI::begin() {
  pinMode(PIN_BTN_UP, INPUT_PULLUP);
  pinMode(PIN_BTN_SEL, INPUT_PULLUP);
  pinMode(PIN_BTN_DOWN, INPUT_PULLUP);

  u8g2_.begin(/* menu_select_pin= */ PIN_BTN_SEL, 
    /* menu_next_pin= */ U8X8_PIN_NONE, /* menu_prev_pin= */ U8X8_PIN_NONE, 
    /* menu_up_pin= */ PIN_BTN_UP, /* menu_down_pin= */ PIN_BTN_DOWN, 
    /* menu_home_pin= */ U8X8_PIN_NONE);  
  
    // mui.begin(u8g2, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));
  // mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);
}

void UI::loop() {
  u8g2_.clearBuffer();
 
  progressBar_.draw(u8g2_);


  u8g2_.sendBuffer();
}

void UI::testStep() {
  progressBar_.setProgress(testProgress_++);
  if (testProgress_ > 100) {
    testProgress_ = 0;
  }
}