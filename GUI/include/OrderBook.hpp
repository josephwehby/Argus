#pragma once

#include <algorithm>
#include <map>
#include <iostream>
#include <memory>

#include "implot.h"
#include "implot_internal.h"
#include "Widget.hpp"
#include "Utils.hpp"
#include "DataStore.hpp"
#include "JsonBuilder.hpp"
#include "Colors.hpp"
#include "BookSnapshot.hpp"
#include "OrderBookManager.hpp"
#include "HttpsClient.hpp"
#include "SubscriptionManager.hpp"

class OrderBook : public Widget {
  public:
    OrderBook(SubscriptionManager&, HttpsClient&, std::string);
    ~OrderBook();
    void init() override;
    void draw() override; 
  private:
    void drawClassic(ImDrawList*);
    void drawDepth(ImDrawList*);

    const unsigned int depth = 20;
    const unsigned int bin_size = 5;
    const float min_bar_height = 20.0f;
    const float min_depth_width = 20.0f; 
    const float max_height = 8.0f;

    bool classic = true;
    BookSnapshot book;
    std::shared_ptr<OrderBookManager> book_manager;
};
