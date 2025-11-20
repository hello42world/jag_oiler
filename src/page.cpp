#include "page.h"
#include "event.h"

void Page::publishEvent(std::unique_ptr<Event> event) {
  eventBus_->publish(std::move(event));
}

bool Page::handleEvent(const Event* event) {
  return false;
}

PageClosedEvent::PageClosedEvent(Page* page)
  : Event(EventID::PageClosed)
  , page(page) {
}
