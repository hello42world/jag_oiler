#pragma once
#include <memory>
#include <queue>

struct Event {
  virtual ~Event() = default;
};


class EventBus {
public:
  EventBus() = default;
  void publish(std::unique_ptr<Event> event);
  std::unique_ptr<Event> consume();
private:
  std::queue<std::unique_ptr<Event>> queue_;
};