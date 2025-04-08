#pragma once

#include <map>
#include <iostream>

#include "Widget.hpp"
#include "Utils.hpp"
#include "DataStore.hpp"

class OrderBook : public Widget {
  public:
    OrderBook(DataStore&, std::string);
    ~OrderBook();
    void readData();
    void draw() override; 
  private:
    std::map<double, double, std::greater<double>> bids = { 
      {99.5, 100}, {99.1, 50}, {98.5, 200}, {98.0, 10}, {97.8, 60},
      {97.5, 150}, {97.2, 120}, {97.0, 90}, {96.8, 110}, {96.5, 75} 
    };
    std::map<double, double> asks = { 
      {100.0, 200}, {100.50, 30}, {100.75, 20}, {101.0, 80}, {101.5, 30},
      {101.8, 60}, {102.0, 50}, {102.2, 90}, {102.5, 120}, {102.8, 100}
    };
    const float bar_height = 20;
    const float x_start = 15;
    const float max_width = 200;
    
    DataStore& datastore;
};
