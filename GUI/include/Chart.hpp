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
#include "Colors.hpp"

class Chart : public Widget {
  public:
    Chart(std::shared_ptr<DataStore>, std::shared_ptr<WebSocket>, std::string);
    ~Chart();
    void draw() override;
  private:
    void updateCandles();
    void drawCandles();
    void drawLine();
    
    const ImVec4 Crosshair_Color = ImVec4{1,1,1,0.5f};

    std::string symbol;
    const std::string channel = "ohlc"; 
    float ratios[2] = {2.5, 1};
    bool show_candles = true;

    std::map<long long, Candle> candles;
    std::shared_ptr<DataStore> datastore;
    std::shared_ptr<WebSocket> ws;
};
