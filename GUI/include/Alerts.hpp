#pragma once

#include <vector>

#include "Widget.hpp"
#include "SubscriptionManager.hpp"
#include "IEvent.hpp"

class Alerts : public Widget {
  public:
    Alerts(SubscriptionManager&);
    void init() override;
    void draw() override;
  private:
    SubscriptionManager& sm;
    
    std::vector<std::string> symbols = {"BTCUSDT", "ETHUSDT", "SOLUSDT", "DOGEUSDT", "ETCUSDT"};
    std::vector<std::string> directions = {"Above", "Below"};

    std::string current_symbol = "BTCUSDT";
    double price = 0;
    std::string direction = "Above";    
};
