#pragma once

#include <unordered_map>
#include <functional>

#include "EventBus.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"

class SubscriptionManager {
  public:
    SubscriptionManager(std::shared_ptr<WebSocket>, std::shared_ptr<EventBus>);
    ~SubscriptionManager();
    void subscribe(std::string channel, int64_t, EventCallback);
    void unsubscribe(std::string channel, int64_t);
  private:
    std::unordered_map<std::string, int> subscriptions;
    std::shared_ptr<EventBus> eb;
    std::shared_ptr<WebSocket> ws;
};
