#include "event.h"

void EventBus::publish(std::unique_ptr<Event> event) {
  queue_.push(std::move(event));
}

std::unique_ptr<Event> EventBus::consume() {
  if (queue_.empty()) {
    return nullptr;
  }
  auto event = std::move(queue_.front());
  queue_.pop();
  return event;
}