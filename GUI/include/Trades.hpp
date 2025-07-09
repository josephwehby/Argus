#pragma once

#include <deque>
#include <string>

#include "Trade.hpp"
#include "Widget.hpp"
#include "Utils.hpp"
#include "JsonBuilder.hpp"
#include "Colors.hpp"
#include "SubscriptionManager.hpp"
#include "SubscriptionRequest.hpp"

class Trades : public Widget, public std::enable_shared_from_this<Trades> {
  public:
    Trades(SubscriptionManager&, std::string);
    ~Trades();
    void init() override;
    void draw() override;
  private:
    const std::string channel = "trade";
    std::string event_channel;
    const unsigned int max_capacity = 60;
    double filter = 0;

    std::deque<Trade> trades;
    SubscriptionRequest request;
    SubscriptionManager& sm;
};
