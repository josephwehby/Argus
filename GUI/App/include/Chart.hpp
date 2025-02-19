#pragma once

#include "Widget.hpp"
#include "Candle.hpp"
#include <vector>

class Chart : public Widget {
  public:
    Chart();
    ~Chart();
    void draw() override;
  private:
    const std::string channel = "ohlc"; 
    std::vector<Candle> candles;
};
