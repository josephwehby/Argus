#pragma once

#include "Widget.hpp"
#include "Utils.hpp"
#include <map>
#include <iostream>

class OrderBook : public Widget {
  public:
    OrderBook(std::string);
    ~OrderBook();
    void readData();
    void draw() override; 
  private:
    std::map<double, double, std::greater<double>> bids = { {99.5, 100}, {99.1, 50}, {98.5, 200}, {98, 10}, {97.8, 60} };
    std::map<double, double> asks = { {100, 200}, {100.50, 30}, {100.75, 20}, {101, 80}, {101.5, 30} };
    const float bar_height = 20;
    const float x_start = 15;
    const float max_width = 200;
};
