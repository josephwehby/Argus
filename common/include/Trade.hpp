#pragma once

#include <string>

#include "IEvent.hpp"

enum class TradeSide { Buy, Sell };

struct Trade : public IEvent {
  Trade() = default;

  Trade(TradeSide tradeside, std::string p, std::string s, std::string t) :
    side(tradeside), price(p), size(s), time(t) {}

  TradeSide side;
  std::string price;
  std::string size;
  std::string time;
};
