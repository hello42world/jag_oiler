#include "dispense_page.h"
#include "motor_events.h"
#include "ui/events.h"
#include "ui/xmui.h"
#include "battery.h"

DispensePage::DispensePage(U8G2* u8g2, EventBus* eventBus)
  : Page(u8g2, eventBus)
  , progressBar_(u8g2, "Ready", 10, 15, 108, 15) {
}

bool DispensePage::handleEvent(const Event* event) {
  if (event->id == EventID::FullRedraw) {
    progressBar_.draw();    
  } else if (event->id == EventID::MotorProgress) {
    auto progress = static_cast<const MotorProgressEvent*>(event)->progressPercent;
    progressBar_.setProgress(progress);
    
    if (progress == 0) {
      progressBar_.setLabel("Dispensing...");
    } else if (progress == 100) {
      progressBar_.setLabel("Ready");
      //auto voltage = batteryReadVoltage();
      //progressBar_.setLabel(ui::sp("Ready [ bat=%d.%02d ]", voltage / 1000, voltage % 1000 / 10));
      progressBar_.setProgress(0);
    }
  } else if (event->id == EventID::Button 
    && static_cast<const ui::ButtonEvent*>(event)->button == U8X8_MSG_GPIO_MENU_SELECT) {
    publishEvent(std::make_unique<PageClosedEvent>(this));
  } else {
    return false;
  }
  return true;
}
