#pragma once

#include "App.hpp"
#include "Widget.hpp"
#include "Level1.hpp"
#include "Utils.hpp"
#include "JsonBuilder.hpp"
#include "Colors.hpp"
#include "IEvent.hpp"
#include "SubscriptionManager.hpp"
#include "SubscriptionRequest.hpp"

class Ticker : public Widget, public std::enable_shared_from_this<Ticker> {
  public:
    Ticker(SubscriptionManager&, std::string);
    ~Ticker();
    void init() override;
    void draw() override; 
  private:
    std::string event_channel;
    const std::string channel = "ticker";
    const float bar_height = 20;
    
    Level1 level1;
    
    SubscriptionRequest request;
    SubscriptionManager& sm;
};
