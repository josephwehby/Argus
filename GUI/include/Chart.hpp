#pragma once

#include <algorithm>
#include <ctime>
#include <vector>
#include <memory>
#include <map>

#include "implot.h"
#include "implot_internal.h"
#include "Widget.hpp"
#include "Candle.hpp"
#include "Utils.hpp"
#include "DataStore.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"

class Chart : public Widget {
  public:
    Chart(std::shared_ptr<DataStore>, std::shared_ptr<WebSocket>, std::string);
    ~Chart();
    void draw() override;
  private:
    void updateCandles();

    std::string symbol;
    const std::string channel = "ohlc"; 
    float ratios[2] = {2.5, 1};
    ImU32 green = IM_COL32(0, 255, 0, 255);
    ImU32 red = IM_COL32(255, 0, 0, 255);
    
    std::map<long long, Candle> candles;
    std::shared_ptr<DataStore> datastore;
    std::shared_ptr<WebSocket> ws;
};
