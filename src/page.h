#pragma once
#include "event.h"
#include <U8g2lib.h>

class Page;

struct PageManager {
  virtual U8G2* u8g2() = 0;
  virtual EventBus* eventBus() = 0;
  virtual bool isActive(Page* page) = 0;
  virtual ~PageManager() = default;
};

class Page {
public:
  Page(PageManager* pageManager);

  virtual void loop() {};
  virtual bool handleEvent(const Event* event);
protected:
  void publishEvent(std::unique_ptr<Event> event);
  
  PageManager* pageManager_;
private:
};

struct PageActivatedEvent : public Event {
  PageActivatedEvent();
};

struct PageClosedEvent : public Event {
  PageClosedEvent(Page* page, int8_t result_ = 0);
  Page* page;
  int8_t result;
};