#pragma once

#include <map>
#include <iostream>
#include <memory>

#include "Widget.hpp"
#include "Utils.hpp"
#include "DataStore.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"

class OrderBook : public Widget {
  public:
    OrderBook(std::shared_ptr<DataStore>, std::shared_ptr<WebSocket>, std::string);
    ~OrderBook();
    void updateBook();
    void draw() override; 
  private:
    const float bar_height = 26.0f;
    
    const ImU32 ask_bar_color = IM_COL32(255, 64, 64, 153);
    const ImU32 ask_text_color = IM_COL32(255, 102, 102, 255);
    const ImU32 bid_bar_color = IM_COL32(51, 204, 102, 153);
    const ImU32 bid_text_color = IM_COL32(102, 255, 153, 255);

    const unsigned int depth = 25;
    const unsigned int depth_view = 20;

    const std::string channel = "book";
    std::map<double, double, std::greater<double>> bids;
    std::map<double, double, std::greater<double>> asks;
    
    std::shared_ptr<DataStore> datastore;
    std::shared_ptr<WebSocket> ws;
};
