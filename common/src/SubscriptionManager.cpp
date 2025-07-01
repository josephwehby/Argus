#include "SubscriptionManager.hpp"

SubscriptionManager::SubscriptionManager(std::shared_ptr<WebSocket> ws_, std::shared_ptr<EventBus> eb_) : ws(ws_), eb(eb_) { }

SubscriptionManager::~SubscriptionManager() {

}

void SubscriptionManager::subscribe(std::string& channel, int64_t id, EventCallback callback) {
  if (!subscriptions.contains(channel)) {
    // generate json sub
    // sub over ws
    // add new subscription
  } else {
    subscriptions[channel]++;
  }

  // register callback with eventbus
  eb->subscribe(channel, id, callback);
}

void SubscriptionManager::unsubscribe() {

}
