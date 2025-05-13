#pragma once

#include <algorithm>
#include <map>
#include <iostream>
#include <memory>

#include "Widget.hpp"
#include "Utils.hpp"
#include "DataStore.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"
#include "Colors.hpp"

class OrderBook : public Widget {
  public:
    OrderBook(std::shared_ptr<DataStore>, std::shared_ptr<WebSocket>, std::string);
    ~OrderBook();
    void updateBook();
    void draw() override; 
  private:
    const unsigned int depth = 25;
    const unsigned int depth_view = 20;
    const float min_bar_height = 20.0f;

    const std::string channel = "book";
    std::map<double, double, std::greater<double>> bids;
    std::map<double, double, std::greater<double>> asks;
    
    std::shared_ptr<DataStore> datastore;
    std::shared_ptr<WebSocket> ws;
};
