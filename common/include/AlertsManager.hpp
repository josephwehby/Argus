#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

#include "Alert.hpp"
#include "SubscriptionRequest.hpp"
#include "SubscriptionManager.hpp"
#include "IEvent.hpp"

class AlertsManager : public std::enable_shared_from_this<AlertsManager> {
  public:
    AlertsManager(SubscriptionManager&);
    ~AlertsManager();
    void addAlert(const Alert&);
    void removeAlert(const int64_t);

    std::unordered_map<uint64_t, Alert> alerts; 
  private:
    SubscriptionManager& sm;
    std::unordered_map<uint64_t, SubscriptionRequest> requests; 
};
