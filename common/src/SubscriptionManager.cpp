#include "SubscriptionManager.hpp"

SubscriptionManager::SubscriptionManager(std::shared_ptr<WebSocket> ws_, std::shared_ptr<EventBus> eb_) : ws(ws_), eb(eb_) { }

void SubscriptionManager::subscribe(const SubscriptionRequest& request) {

  if (!subscriptions.contains(request.channel)) {

    json sub_msg;
    if (request.params.has_value()) {
      sub_msg = JsonBuilder::generateSubscribe(request.symbol, request.channel, request.id, request.params.value());
    } else {
      sub_msg = JsonBuilder::generateSubscribe(request.symbol, request.channel, request.id);
    }

    ws->subscribe(sub_msg);
  }
  
  subscriptions[request.channel]++;

  eb->subscribe(request.event_channel, request.id, request.callback);
}

void SubscriptionManager::unsubscribe(SubscriptionRequest request) {

  if (!subscriptions.contains(request.channel)) return;
  
  if (subscriptions[request.channel] == 1) {
    
    json unsub_msg;
    if (request.params.has_value()) {
      unsub_msg = JsonBuilder::generateUnsubscribe(request.symbol, request.channel, request.id, request.params.value());
    } else {
      unsub_msg = JsonBuilder::generateUnsubscribe(request.symbol, request.channel, request.id);
    }

    ws->unsubscribe(unsub_msg);
    subscriptions.erase(request.channel);
  } else {
    subscriptions[request.channel]--;
  }

  eb->unsubscribe(request.event_channel, request.id);
}
