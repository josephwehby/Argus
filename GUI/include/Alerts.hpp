#pragma once

#include <vector>
#include <limits>
#include <chrono>
#include <cstdint>
#include <random>
#include <iostream>
#include <memory>

#include "PlaySoundWrapper.hpp"
#include "Widget.hpp"
#include "AlertsManager.hpp"
#include "Colors.hpp"

class Alerts : public Widget {
  public:
    Alerts(SubscriptionManager&);
    void init() override;
    void draw() override;
  private:
    uint64_t genID();
 
    std::vector<std::string> symbols = {"BTCUSDT", "ETHUSDT", "SOLUSDT", "DOGEUSDT", "ETCUSDT"};
    std::vector<std::string> directions = {"Above", "Below"};

    std::string current_symbol = "BTCUSDT";
    double price = 0;
    std::string direction = "Above";

  std::shared_ptr<AlertsManager> am;
};
