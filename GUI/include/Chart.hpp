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
#include "SubscriptionManager.hpp"
#include "SubscriptionRequest.hpp"
#include "HttpsClient.hpp"
#include "JsonBuilder.hpp"
#include "Colors.hpp"

class Chart : public Widget, public std::enable_shared_from_this<Chart> {
  public:
    Chart(SubscriptionManager&, HttpsClient&, std::string);
    ~Chart();
    void init() override;
    void draw() override;
  private:
    void updateCandles();
    void setStyle();
    void initDraw();
    void drawCandles(ImDrawList*, double);
    void drawLine(ImDrawList*);
    void drawVolume(double);
    void changeTimeFrame(std::string);

    const ImVec4 Crosshair_Color = ImVec4{1,1,1,0.5f};

    std::string symbol;
    const std::string channel = "kline"; 
    std::string event_channel;
    std::string historical_candles_event_channel;
    std::string time_frame = "1m";
    
    float ratios[2] = {2.5, 1};
    long long candles_before_load = 5;
    bool loading_data = true;
    bool show_candles = true;
    
    std::map<long long, Candle> candles;
    std::vector<double> lines;

    SubscriptionManager& sm;
    HttpsClient& hc;

    SubscriptionRequest request;
    SubscriptionRequest historical_request;
};
