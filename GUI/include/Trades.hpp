#pragma once

#include <deque>
#include <string>

#include "Trade.hpp"
#include "Widget.hpp"
#include "Utils.hpp"
#include "DataStore.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"

class Trades : public Widget {
  public:
    Trades(DataStore&, std::shared_ptr<WebSocket>, std::string);
    ~Trades();
    void draw() override;
  private:
    const std::string channel = "trade";
    unsigned int max_capacity = 40;

    std::shared_ptr<WebSocket> ws;
    std::deque<Trade> trades;
    DataStore& datastore;
};
