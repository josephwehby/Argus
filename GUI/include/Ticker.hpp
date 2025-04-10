#pragma once

#include "Widget.hpp"
#include "Level1.hpp"
#include "Utils.hpp"
#include "DataStore.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"

class Ticker : public Widget {
  public:
    Ticker(DataStore&, std::shared_ptr<WebSocket>, std::string);
    void draw() override; 
  private:
    const std::string channel = "ticker";
    const float bar_length = 400;
    const float bar_height = 20;
    const float x_start = 17.5;
    const float y_start = 30;
    const float y_bars_start = 50;
    
    std::shared_ptr<Level1> level1;
    DataStore& datastore;
    std::shared_ptr<WebSocket> ws;
};
