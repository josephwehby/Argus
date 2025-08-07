#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include <iostream>

#include "EventBus.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"
#include "SubscriptionRequest.hpp"

class SubscriptionManager {
public:
  SubscriptionManager(EventBus&);
  ~SubscriptionManager();
  void setWebSocket(std::shared_ptr<WebSocket>);
  void subscribe(const SubscriptionRequest&);
  void unsubscribe(const SubscriptionRequest&);
private:
  std::unordered_map<std::string, int> subscriptions;

  std::shared_ptr<WebSocket> ws;
  EventBus& eb;
};
