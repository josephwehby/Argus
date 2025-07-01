#pragma once

#include <unordered_map>
#include <functional>

#include "EventBus.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"
#include "SubscriptionRequest.hpp"

class SubscriptionManager {
  public:
    SubscriptionManager(std::shared_ptr<WebSocket>, std::shared_ptr<EventBus>);
    void subscribe(const SubscriptionRequest&);
    void unsubscribe(const SubscriptionRequest&);
  private:
    std::unordered_map<std::string, int> subscriptions;
    std::shared_ptr<EventBus> eb;
    std::shared_ptr<WebSocket> ws;
};
