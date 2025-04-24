#pragma once

struct Candle {

  Candle() {}

  Candle(double o, double h, double l, double c, double v, unsigned int i, std::string b, long long ut) :
    open(o), high(h), low(l), close(c), volume(v), interval(i), begin(b), unix_time(ut) {}

  double open;
  double high;
  double low;
  double close;
  double volume;
  unsigned int interval;
  std::string begin;
  long long unix_time;
};
