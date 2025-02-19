#pragma once

struct Candle {
  double open;
  double high;
  double low;
  double close;
  double volume;
  unsigned int interval;
  std::string begin;
};
