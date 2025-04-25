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
    
    const ImVec4 limitBuyColor  = {0.f, 0.4f, 0.f, 1.f};
    const ImVec4 marketBuyColor = {0.f, 1.f, 0.3f, 1.f};

    const ImVec4 limitSellColor = {0.5f, 0.05f, 0.05f, 1.f};
    const ImVec4 marketSellColor = {1.0f, 0.1f, 0.1f, 1.f};

    std::shared_ptr<WebSocket> ws;
    std::deque<Trade> trades;
    std::shared_ptr<DataStore> datastore;
};
