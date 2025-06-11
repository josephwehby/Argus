#pragma once

#include "App.hpp"
#include "Widget.hpp"
#include "Level1.hpp"
#include "Utils.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"
#include "Colors.hpp"
#include "EventBus.hpp"
#include "IEvent.hpp"

class Ticker : public Widget {
  public:
    Ticker(std::shared_ptr<WebSocket>, std::shared_ptr<EventBus>, std::string);
    ~Ticker();
    void draw() override; 
  private:
    std::string event_channel;
    const std::string channel = "ticker";
    const float bar_height = 20;
    
    Level1 level1;
    
    std::shared_ptr<WebSocket> ws;
    std::shared_ptr<EventBus> eb;
};
