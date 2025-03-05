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
      {TradeType::Buy,  98750.0,  1500, "2025-02-19T09:30:00.123Z"},
      {TradeType::Sell, 98800.0,  2000, "2025-02-19T09:30:05.456Z"},
      {TradeType::Buy,  98780.0,  1200, "2025-02-19T09:30:10.789Z"},
      {TradeType::Sell, 98850.0,  1750, "2025-02-19T09:30:16.012Z"},
      {TradeType::Buy,  98770.0,  3000, "2025-02-19T09:30:21.345Z"},
      {TradeType::Sell, 98820.0,  2200, "2025-02-19T09:30:25.678Z"},
      {TradeType::Buy,  98790.0,  1800, "2025-02-19T09:30:30.901Z"},
      {TradeType::Sell, 98860.0,  1400, "2025-02-19T09:30:36.234Z"},
      {TradeType::Buy,  98760.0,  2500, "2025-02-19T09:30:41.567Z"},
      {TradeType::Sell, 98880.0,  1000, "2025-02-19T09:30:45.890Z"},
      {TradeType::Buy,  98740.0,  900,  "2025-02-19T09:30:50.123Z"},
      {TradeType::Sell, 98900.0,  3100, "2025-02-19T09:30:55.456Z"},
      {TradeType::Buy,  98720.0,  4000, "2025-02-19T09:31:00.789Z"},
      {TradeType::Buy, 100910.0,  3500, "2025-02-19T09:31:06.012Z"},
      {TradeType::Sell, 101200.0,  2800, "2025-02-19T09:31:10.234Z"},
      {TradeType::Buy, 100850.0,  3100, "2025-02-19T09:31:15.567Z"},
      {TradeType::Sell, 101300.0,  2900, "2025-02-19T09:31:20.890Z"},
      {TradeType::Buy, 100900.0,  3300, "2025-02-19T09:31:25.123Z"},
      {TradeType::Sell, 101500.0,  2700, "2025-02-19T09:31:30.456Z"},
      {TradeType::Buy, 100920.0,  3500, "2025-02-19T09:31:35.789Z"},
      {TradeType::Sell, 101550.0,  3800, "2025-02-19T09:31:40.012Z"},
      {TradeType::Buy, 100950.0,  3600, "2025-02-19T09:31:45.345Z"},
      {TradeType::Sell, 101600.0,  3900, "2025-02-19T09:31:50.678Z"},
      {TradeType::Buy, 100980.0,  3200, "2025-02-19T09:31:55.901Z"},
      {TradeType::Sell, 101650.0,  4100, "2025-02-19T09:32:00.234Z"},
      {TradeType::Buy, 101000.0,  3700, "2025-02-19T09:32:05.567Z"},
      {TradeType::Sell, 101700.0,  4300, "2025-02-19T09:32:10.890Z"},
      {TradeType::Buy, 101020.0,  3500, "2025-02-19T09:32:15.123Z"},
      {TradeType::Sell, 101750.0,  4500, "2025-02-19T09:32:20.456Z"},
      {TradeType::Buy, 101050.0,  3900, "2025-02-19T09:32:25.789Z"}
        };
};
