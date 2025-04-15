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
    void updateBook();
    void draw() override; 
  private:
    const float bar_height = 20.0f;
    const float x_start = 15.0f;
    const float max_width = 400.0f;
    const float y_start = 30.0f;
    const float min_width = 3.0f;

    const unsigned int depth = 10;
    const std::string channel = "book";
    std::map<double, double, std::greater<double>> bids;
    std::map<double, double, std::greater<double>> asks;
    
    DataStore& datastore;
    std::shared_ptr<WebSocket> ws;
};
