#include "prime_pump_page.h"
#include "ui/events.h"
#include "motor.h"
#include "motor_events.h"

PrimePumpPage::PrimePumpPage(U8G2* u8g2, EventBus* eventBus)
  : Page(u8g2, eventBus)
  , state_(State::Ready)
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
  } else if (event->id == EventID::MotorProgress) {
    auto progressEvt = static_cast<const MotorProgressEvent*>(event);
    if (progressEvt->progressPercent == 0) {
      state_ = progressEvt->forward ? State::Priming : State::Flushing;
      draw();
    } else if (progressEvt->progressPercent == 100) {
      state_ = State::Ready;
      draw();
    }
  } else {
    return false;
  }
  return true;
}


void PrimePumpPage::draw() {
  u8g2_->clearBuffer();
  u8g2_->setFont(u8g2_font_helvR10_tr);
  if (state_ == State::Ready) {
    printInCenter("Ready");
  } else if (state_ == State::Priming) {
    printInCenter("Priming...");
  } else if (state_ == State::Flushing) {
    printInCenter("Flushing...");
  }
  u8g2_->sendBuffer();
}

void PrimePumpPage::printInCenter(const char* str) {
  u8g2_->setFont(u8g2_font_helvR10_tr);
  u8g2_uint_t x = (u8g2_->getWidth() - u8g2_->getStrWidth(str)) / 2;
  u8g2_->drawStr(x, (u8g2_->getHeight() + 10) / 2, str);
}

