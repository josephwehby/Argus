#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>
#include <utility>

#include "IEvent.hpp"
#include "SafeQueue.hpp"

using EventCallback = std::function<void(std::shared_ptr<IEvent>)>;

class EventBus {
  public:
    EventBus() = default;
    
    void subscribe(const std::string& channel, int64_t, EventCallback);
    void unsubscribe(const std::string& channel, int64_t);
    void deferUnsubscribe(const std::string& channel, int64_t);

    void publish(std::shared_ptr<IEvent>);
    void dispatchAll();
  private:
    std::unordered_map<std::string, std::unordered_map<int64_t, EventCallback>> subscriptions;
    SafeQueue<std::shared_ptr<IEvent>> events;
    SafeQueue<std::pair<std::string, int64_t>> unsubs;
};
