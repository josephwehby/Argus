#pragma once

#include <algorithm>
#include <ctime>
#include <vector>

#include "implot.h"
#include "implot_internal.h"
#include "Widget.hpp"
#include "Candle.hpp"
#include "Utils.hpp"
#include "DataStore.hpp"

class Chart : public Widget {
  public:
    Chart(DataStore&, std::string);
    ~Chart();
    void draw() override;
  private:
    std::string symbol;
    const std::string channel = "ohlc"; 
    float ratios[2] = {2.5, 1};
    ImU32 green = IM_COL32(0, 255, 0, 255);
    ImU32 red = IM_COL32(255, 0, 0, 255);
    std::vector<Candle> candles;
    DataStore& datastore;
};
