#include "dispense_page.h"
#include "dispense_events.h"
#include "ui/events.h"

DispensePage::DispensePage(U8G2* u8g2)
  : Page(u8g2) {
}

void DispensePage::loop(const Event* event) {
  if (event->id == EventID::FullRedraw) {
    progressBar_.draw(*u8g2_);    
  } else if (event->id == EventID::Progress) {
    const ProgressEvent* progressEvent = static_cast<const ProgressEvent*>(event);
    bool updated = progressBar_.getProgress() != progressEvent->progressPercent;
    if (updated) {
      progressBar_.setProgress(progressEvent->progressPercent);
      progressBar_.draw(*u8g2_);
      progressBar_.updateDisplay(*u8g2_);
    }
  }
}
