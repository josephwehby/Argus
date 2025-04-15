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
    OrderBook(DataStore&, std::shared_ptr<WebSocket>, std::string);
    ~OrderBook();
    void readData();
    void draw() override; 
  private:
    const float bar_height = 20;
    const float x_start = 15;
    const float max_width = 200;
    
    std::map<double, double, std::greater<double>> bids;
    std::map<double, double> asks;
    const std::string channel = "book";
    DataStore& datastore;
    std::shared_ptr<WebSocket> ws;
};
