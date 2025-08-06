#include "AlertsManager.hpp"

AlertsManager::AlertsManager(SubscriptionManager& sm_) : sm(sm_) {}

AlertsManager::~AlertsManager() {
  for (const auto&[id, request] : requests) {
    sm.unsubscribe(request);
  }

  requests.clear();
  alerts.clear();
}

void AlertsManager::addAlert(const Alert& alert) {
  std::weak_ptr<AlertsManager> self = shared_from_this();
  
  std::string event_channel = "LEVEL1:" + alert.symbol;
  
  SubscriptionRequest request = {
    alert.symbol,
    "ticker",
    event_channel,
    alert.id,
    [self, alert] (const std::shared_ptr<IEvent> update) {
      if (auto locked = self.lock()) {
        auto event = std::dynamic_pointer_cast<Level1>(update);
        if (event->empty) return;
        if (alert.direction == Direction::ABOVE && event->last_price >= alert.price) {
          locked->alerts[alert.id].triggered = true;
        } else if (alert.direction == Direction::BELOW && event->last_price <= alert.price) {
          locked->alerts[alert.id].triggered = true;
        } else {
        
        }
      }
    }, 
    std::nullopt,
    false
  };
  
  alerts[alert.id] = alert;
  requests[alert.id] = request;
  sm.subscribe(request);
}

void AlertsManager::removeSubscription(const int64_t id) {
  if (requests.contains(id)) sm.unsubscribe(requests[id]);
  requests.erase(id);
}

void AlertsManager::removeAlert(const int64_t id) {
  alerts.erase(id);
}
