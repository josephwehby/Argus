#pragma once

#include <string>

enum class TradeSide { Buy, Sell };

struct Trade {
  Trade() {}

  Trade(TradeSide tradeside, std::string p, std::string s, std::string t) :
    side(tradeside), price(p), size(s), time(t) {}

  TradeSide side;
  std::string price;
  std::string size;
  std::string time;
};
