#include "dispense_page.h"
#include "dispense_events.h"
#include "ui/events.h"

DispensePage::DispensePage(U8G2* u8g2, EventBus* eventBus)
  : Page(u8g2, eventBus)
  , progressBar_(u8g2, "Dispensing...", 10, 15, 108, 10) {
}

bool DispensePage::handleEvent(const Event* event) {
  if (event->id == EventID::FullRedraw) {
    progressBar_.draw();    
  } else if (event->id == EventID::PortionProgress) {
    progressBar_.setProgress(static_cast<const PortionProgressEvent*>(event)->progressPercent);
  } else if (event->id == EventID::Button 
    && static_cast<const ui::ButtonEvent*>(event)->button == U8X8_MSG_GPIO_MENU_SELECT) {
    publishEvent(std::make_unique<PageClosedEvent>(this));
  } else {
    return false;
  }
  return true;
}
