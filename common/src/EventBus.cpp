#include "EventBus.hpp"

void EventBus::subscribe(const std::string& channel, int64_t id, EventCallback callback) {
  subscriptions[channel][id] = callback;
}

void EventBus::unsubscribe(const std::string& channel, int64_t id) {
  if (!subscriptions.contains(channel)) return;
  if (!subscriptions[channel].contains(id)) return;

  subscriptions[channel].erase(id);  
}

void EventBus::deferUnsubscribe(const std::string& channel, int64_t id) {
  unsubs.push({channel, id}); 
}

void EventBus::publish(std::shared_ptr<IEvent> event) {
  events.push(event);
}

void EventBus::dispatchAll() {
  size_t size = events.size();
  for (int i = 0; i < size; i++) {
    std::shared_ptr<IEvent> event;
    events.pop(event);
    
    auto it = subscriptions.find(event->channel);
    for (const auto&[_, callback]: it->second) {
      callback(event); 
    }
  }

  size = unsubs.size();
  for (int i = 0; i < size; i++) {
    auto event = unsubs.pop();
    unsubscribe(event->first, event->second); 
  }
}
