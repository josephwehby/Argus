#pragma once

struct Candle {

  Candle() {}

  Candle(double o, double h, double l, double c, double bv, double v, unsigned int i, long long ut) :
    open(o), high(h), low(l), close(c), buy_volume(bv), volume(v), interval(i), unix_time(ut) {}

  double open;
  double high;
  double low;
  double close;
  double buy_volume;
  double volume;
  unsigned int interval;
  long long unix_time;
};
