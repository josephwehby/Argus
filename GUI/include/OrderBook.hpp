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
#include "BookSnapshot.hpp"

class OrderBook : public Widget {
  public:
    OrderBook(std::shared_ptr<WebSocket>, std::string);
    ~OrderBook();
    void updateBook();
    void draw() override; 
  private:
    const unsigned int depth = 20;
    const float min_bar_height = 20.0f;

    const std::string channel = "depth";
    const std::string speed = "100ms";

    BookSnapshot book;

    std::shared_ptr<WebSocket> ws;
//    std::shared_ptr<MasterOrderBookManager> mobm;
};
