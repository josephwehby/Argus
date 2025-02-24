#pragma once

#include "implot.h"
#include "implot_internal.h"
#include "Widget.hpp"
#include "Candle.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <vector>

class Chart : public Widget {
  public:
    Chart();
    ~Chart();
    void draw() override;
  private:
    const std::string channel = "ohlc"; 
    ImU32 green = IM_COL32(0, 255, 0, 255);
    ImU32 red = IM_COL32(255, 0, 0, 255);
    std::vector<Candle> candles;
};
