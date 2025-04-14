#pragma once

#include <string>

enum class TradeType { Limit, Market };
enum class TradeSide { Buy, Sell };

struct Trade {
  Trade() {}

  Trade(TradeType tradetype, TradeSide tradeside, double p, double s, std::string t) :
    type(tradetype), side(tradeside), price(p), size(s), time(t) {}

  TradeType type;
  TradeSide side;
  double price;
  double size;
  std::string time;
};
