#pragma once

#include <deque>
#include <string>

#include "Trade.hpp"
#include "Widget.hpp"
#include "Utils.hpp"

class Trades : public Widget {
  public:
    Trades(std::string);
    ~Trades();
    void draw() override;
  private:
    std::deque<Trade> trades = {
      {TradeType::Buy,  98.75,  150, "2025-02-12T14:32:45.123Z"},
      {TradeType::Sell, 98.80,  200, "2025-02-12T14:32:50.456Z"},
      {TradeType::Buy,  98.78,  120, "2025-02-12T14:32:55.789Z"},
      {TradeType::Sell, 98.85,  175, "2025-02-12T14:33:01.012Z"},
      {TradeType::Buy,  98.77,  300, "2025-02-12T14:33:06.345Z"},
      {TradeType::Sell, 98.82,  220, "2025-02-12T14:33:10.678Z"},
      {TradeType::Buy,  98.79,  180, "2025-02-12T14:33:15.901Z"},
      {TradeType::Sell, 98.86,  140, "2025-02-12T14:33:21.234Z"},
      {TradeType::Buy,  98.76,  250, "2025-02-12T14:33:26.567Z"},
      {TradeType::Sell, 98.88,  100, "2025-02-12T14:33:30.890Z"},
      {TradeType::Buy,  98.74,  90,  "2025-02-12T14:33:35.123Z"},
      {TradeType::Sell, 98.90,  310, "2025-02-12T14:33:40.456Z"},
      {TradeType::Buy,  98.72,  400, "2025-02-12T14:33:45.789Z"},
      {TradeType::Buy,  100.91,  350, "2025-02-12T14:33:51.012Z"},
      {TradeType::Buy,  98.71,  270, "2025-02-12T14:33:56.345Z"},
      {TradeType::Sell, 98.92,  290, "2025-02-12T14:34:01.678Z"},
      {TradeType::Buy,  98.70,  320, "2025-02-12T14:34:06.901Z"},
      {TradeType::Sell, 98.93,  280, "2025-02-12T14:34:12.234Z"},
      {TradeType::Buy,  98.69,  150, "2025-02-12T14:34:17.567Z"},
      {TradeType::Sell, 98.94,  230, "2025-02-12T14:34:22.890Z"}
    };
};
