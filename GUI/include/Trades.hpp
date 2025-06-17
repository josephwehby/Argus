#pragma once

#include <deque>
#include <string>

#include "Trade.hpp"
#include "Widget.hpp"
#include "Utils.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"
#include "Colors.hpp"
#include "EventBus.hpp"

class Trades : public Widget {
  public:
    Trades(std::shared_ptr<WebSocket>, std::shared_ptr<EventBus>, std::string);
    ~Trades();
    void draw() override;
  private:
    const std::string channel = "trade";
    std::string event_channel;
    const unsigned int max_capacity = 60;
    double filter = 0;

    std::shared_ptr<WebSocket> ws;
    std::deque<Trade> trades;
    std::shared_ptr<EventBus> eb;
};
