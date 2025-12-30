#include "prime_pump_page.h"
#include "ui/events.h"
#include "ui/xmui.h"
#include "motor.h"
#include "motor_events.h"

PrimePumpPage::PrimePumpPage(PageManager* pageManager)
  : Page(pageManager)
  , state_(State::Ready)
  , buttonHint_(pageManager->u8g2(), "Flush", "Stop", "Prime")
{
}

bool PrimePumpPage::handleEvent(const Event* event) {
  if (event->id == EventID::FullRedraw) {
    draw();
  } else if (event->id == EventID::Button) {
    const ui::ButtonEvent* buttonEvent = static_cast<const ui::ButtonEvent*>(event);
    if (buttonEvent->button == U8X8_MSG_GPIO_MENU_SELECT) {
      publishEvent(std::make_unique<MotorStopCommandEvent>());
    } else if (buttonEvent->button == U8X8_MSG_GPIO_MENU_HOME) {
      publishEvent(std::make_unique<MotorStopCommandEvent>());
      publishEvent(std::make_unique<PageClosedEvent>(this));
    } else if (buttonEvent->button == U8X8_MSG_GPIO_MENU_NEXT) {
      publishEvent(std::make_unique<MotorStartCommandEvent>(Motor::TURN_STEPS * 10));
    } else if (buttonEvent->button == U8X8_MSG_GPIO_MENU_PREV) {
      publishEvent(std::make_unique<MotorStartCommandEvent>(-Motor::TURN_STEPS * 10));
    }
  } else if (event->id == EventID::MotorProgress && pageManager_->isActive(this)) {
    auto progressEvt = static_cast<const MotorProgressEvent*>(event);
    if (progressEvt->progressPercent == 0) {
      state_ = progressEvt->forward ? State::Priming : State::Flushing;
      publishEvent(std::make_unique<ui::FullRedrawEvent>());
    } else if (progressEvt->progressPercent == 100) {
      state_ = State::Ready;
      publishEvent(std::make_unique<ui::FullRedrawEvent>());
    }
  } else {
    return false;
  }
  return true;
}


void PrimePumpPage::draw() {
  if (state_ == State::Ready) {
    printInCenter("Ready");
  } else if (state_ == State::Priming) {
    printInCenter("Priming...");
  } else if (state_ == State::Flushing) {
    printInCenter("Flushing...");
  }
  buttonHint_.draw();
}

void PrimePumpPage::printInCenter(const char* str) {
  pageManager_->u8g2()->setFont(XMUI_BIG_FONT);
  u8g2_uint_t x = (pageManager_->u8g2()->getWidth() - pageManager_->u8g2()->getStrWidth(str)) / 2;
  pageManager_->u8g2()->drawStr(x, (pageManager_->u8g2()->getHeight() + 10) / 2, str);
}

