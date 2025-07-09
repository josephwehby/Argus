#include "SubscriptionManager.hpp"

SubscriptionManager::SubscriptionManager(EventBus& eb_) : eb(eb_) { }

SubscriptionManager::~SubscriptionManager() {
  std::cout << "sm destructor ran" << std::endl;
}

void SubscriptionManager::setWebSocket(std::shared_ptr<WebSocket> ws_) { ws = ws_; }

void SubscriptionManager::subscribe(const SubscriptionRequest& request) {

  if (!subscriptions.contains(request.event_channel)) {

    json sub_msg;
    if (request.params.has_value()) {
      sub_msg = JsonBuilder::generateSubscribe(request.symbol, request.channel, request.id, request.params.value());
    } else {
      sub_msg = JsonBuilder::generateSubscribe(request.symbol, request.channel, request.id);
    }
    
    ws->subscribe(sub_msg);
  }

  subscriptions[request.event_channel]++;
  eb.subscribe(request.event_channel, request.id, request.callback);
}

void SubscriptionManager::unsubscribe(const SubscriptionRequest& request) {

  if (!subscriptions.contains(request.event_channel)) return;
  
  if (subscriptions[request.event_channel] == 1) {
    
    json unsub_msg;
    if (request.params.has_value()) {
      unsub_msg = JsonBuilder::generateUnsubscribe(request.symbol, request.channel, request.id, request.params.value());
    } else {
      unsub_msg = JsonBuilder::generateUnsubscribe(request.symbol, request.channel, request.id);
    }

    ws->unsubscribe(unsub_msg);
    subscriptions.erase(request.event_channel);
  } else {
    subscriptions[request.event_channel]--;
  }

  eb.unsubscribe(request.event_channel, request.id);
}
