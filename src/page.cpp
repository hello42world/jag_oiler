#include "page.h"
#include "event.h"
//
// Page
//

Page::Page(PageManager* pageManager)
  : pageManager_(pageManager) {
}

void Page::publishEvent(std::unique_ptr<Event> event) {
   pageManager_->eventBus()->publish(std::move(event));
}

bool Page::handleEvent(const Event* event) {
  return false;
}

//
// PageActivatedEvent
//
PageActivatedEvent::PageActivatedEvent()
  : Event(EventID::PageActivated) {
}


//
// PageClosedEvent
//
PageClosedEvent::PageClosedEvent(Page* page, int8_t result_)
  : Event(EventID::PageClosed)
  , page(page)
  , result(result_) {
}
