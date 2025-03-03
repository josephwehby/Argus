#pragma once

#include <string>

enum class TradeType { Buy, Sell };

struct Trade {
  TradeType type;
  double price;
  int size;
  std::string time;
};
