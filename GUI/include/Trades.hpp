#pragma once

#include <deque>
#include <string>

#include "Trade.hpp"
#include "Widget.hpp"
#include "Utils.hpp"
#include "DataStore.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"
#include "Colors.hpp"

class Trades : public Widget {
  public:
    Trades(std::shared_ptr<DataStore>, std::shared_ptr<WebSocket>, std::string);
    ~Trades();
    void draw() override;
  private:
    const std::string channel = "trade";
    unsigned int max_capacity = 60;

    std::shared_ptr<WebSocket> ws;
    std::deque<Trade> trades;
    std::shared_ptr<DataStore> datastore;
};
