#pragma once
#include "event.h"
#include <U8g2lib.h>

class Page {
public:
  Page(U8G2* u8g2, EventBus* eventBus) 
    : u8g2_(u8g2), eventBus_(eventBus) {}
  
  virtual void loop() {};
  virtual bool handleEvent(const Event* event);
protected:
  void publishEvent(std::unique_ptr<Event> event);
  U8G2 *u8g2_;
private:
  EventBus *eventBus_;
};

struct PageClosedEvent : public Event {
  PageClosedEvent(Page* page);
  Page* page;
};