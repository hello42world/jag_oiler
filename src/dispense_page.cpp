#include "dispense_page.h"
#include "motor_events.h"
#include "menu_events.h"
#include "ui/events.h"
#include "ui/xmui.h"
#include "battery.h"
#include "motor.h"

DispensePage::DispensePage(PageManager* pageManager, int8_t dropSize)
  : Page(pageManager)
  , progressBar_(pageManager->u8g2(), "Ready", 10, 30, 108, 15)
  , dropSize_(dropSize) {
}

bool DispensePage::handleEvent(const Event* event) {
  if (event->id == EventID::FullRedraw) {
    progressBar_.draw();
    drawInfo();
  } else if (event->id == EventID::MotorProgress && pageManager_->isActive(this)) {
    auto progress = static_cast<const MotorProgressEvent*>(event)->progressPercent;
    progressBar_.setProgress(progress);
    
    if (progress == 0) {
      progressBar_.setLabel("Dispensing...");
    } else if (progress == 100) {
      progressBar_.setLabel("Ready");
      progressBar_.setProgress(0);

      dropsDispensed_ += 1;
      dropUnitsDispensed_ += dropSize_;
      
      publishEvent(std::make_unique<ui::FullRedrawEvent>());
    }
  } else if (event->id == EventID::SettingsChanged) {
    const SettingsChangedEvent* settingsEvent = static_cast<const SettingsChangedEvent*>(event);
    dropSize_ = settingsEvent->settings.dropSize;
  } else if (event->id == EventID::Button) {
    const auto* buttonEvent = static_cast<const ui::ButtonEvent*>(event);
    if (buttonEvent->button == U8X8_MSG_GPIO_MENU_SELECT) {
      publishEvent(std::make_unique<PageClosedEvent>(this));
    } else if (buttonEvent->button == U8X8_MSG_GPIO_MENU_HOME) {
      int32_t steps = dropSize_ * Motor::TURN_STEPS / 4;
      publishEvent(std::make_unique<MotorStartCommandEvent>(steps));
      Serial.printf("DispensePage: Dispensing %d drops (%d steps)\n", dropSize_, steps);
    }
  } else {
    return false;
  }
  return true;
}

void DispensePage::drawInfo() {
  pageManager_->u8g2()->setFont(XMUI_DEFAULT_FONT);
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "DS:%d | Cnt:%d/%d", dropSize_, dropsDispensed_, dropUnitsDispensed_);
  int16_t y = pageManager_->u8g2()->getDisplayHeight() - 0;
  pageManager_->u8g2()->drawStr(0, y, buffer);
}