#pragma once

#include <string>

enum class TradeType { Limit, Market };
enum class TradeSide { Buy, Sell };

struct Trade {
  Trade() {}

  Trade(TradeType tradetype, TradeSide tradeside, std::string p, std::string s, std::string t) :
    type(tradetype), side(tradeside), price(p), size(s), time(t) {}

  TradeType type;
  TradeSide side;
  std::string price;
  std::string size;
  std::string time;
};
