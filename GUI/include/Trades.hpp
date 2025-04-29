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
    Trades(std::shared_ptr<DataStore>, std::shared_ptr<WebSocket>, std::string);
    ~Trades();
    void draw() override;
  private:
    const std::string channel = "trade";
    unsigned int max_capacity = 40;
    
    const ImVec4 market_buy_color = {153/255.f, 1.f, 204/255.f, 1.f};
    const ImVec4 limit_buy_color = {102/255.f, 1.f, 153/255.f, 1.f};

    const ImVec4 market_sell_color = {1.f, 153/255.f, 153/255.f, 1.f};
    const ImVec4 limit_sell_color = {1.f, 102/255.f, 102/255.f, 1.f};

    std::shared_ptr<WebSocket> ws;
    std::deque<Trade> trades;
    std::shared_ptr<DataStore> datastore;
};
