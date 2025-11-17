#pragma once
#include <memory>
#include <queue>


enum class EventID : uint8_t {
  FullRedraw,
  Button,
  PortionProgress
}; 

struct Event {
  Event() = delete;
  Event(EventID eventId) : id(eventId) {}

  const EventID id;

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