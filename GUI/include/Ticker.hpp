#pragma once

#include "App.hpp"
#include "Widget.hpp"
#include "Level1.hpp"
#include "Utils.hpp"
#include "DataStore.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"
#include "Colors.hpp"

class Ticker : public Widget {
  public:
    Ticker(std::shared_ptr<DataStore>, std::shared_ptr<WebSocket>, std::string);
    ~Ticker();
    void draw() override; 
  private:
    const std::string channel = "ticker";
    
    const float bar_height = 20;
    
    std::shared_ptr<Level1> level1;
    std::shared_ptr<DataStore> datastore;
    std::shared_ptr<WebSocket> ws;
};
