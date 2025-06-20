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
#include "HistoricalCandles.hpp"
#include "Utils.hpp"
#include "WebSocket.hpp"
#include "EventBus.hpp"
#include "HttpsClient.hpp"
#include "DataParser.hpp"
#include "JsonBuilder.hpp"
#include "Colors.hpp"

class Chart : public Widget {
  public:
    Chart(std::shared_ptr<WebSocket>, std::shared_ptr<EventBus>, std::shared_ptr<HttpsClient>, std::string);
    ~Chart();
    void draw() override;
  private:
    void updateCandles();
    void setStyle();
    void drawCandles();
    void drawLine();
    void drawVolume(double);
    void changeTimeFrame(std::string);

    const ImVec4 Crosshair_Color = ImVec4{1,1,1,0.5f};

    std::string symbol;
    const std::string channel = "kline"; 
    std::string event_channel;
    std::string historical_candles_event_channel;
    std::string time_frame = "1m";

    float ratios[2] = {2.5, 1};
    bool show_candles = true;
    
    std::map<long long, Candle> candles;
    std::shared_ptr<WebSocket> ws;
    std::shared_ptr<EventBus> eb;
    std::shared_ptr<HttpsClient> hc;
};
