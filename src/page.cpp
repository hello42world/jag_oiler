#include "page.h"
#include "event.h"

void Page::publishEvent(std::unique_ptr<Event> event) {
  eventBus_->publish(std::move(event));
}

bool Page::handleEvent(const Event* event) {
  return false;
}

PageClosedEvent::PageClosedEvent(Page* page, int8_t result_)
  : Event(EventID::PageClosed)
  , page(page)
  , result(result_) {
}
