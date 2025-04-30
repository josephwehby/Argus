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

    const ImU32 green = IM_COL32(102, 255, 153, 255);
    const ImU32 red = IM_COL32(255, 102, 102, 255);
    const ImVec4 background_color = ImVec4{40/255.f, 42/255.f, 54/255.f, 1.0f};

    std::map<long long, Candle> candles;
    std::shared_ptr<DataStore> datastore;
    std::shared_ptr<WebSocket> ws;
};
