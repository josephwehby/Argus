#pragma once

#include "IEvent.hpp"

struct Candle : public IEvent {

  Candle() {}

  Candle(double o, double h, double l, double c, double bv, double v, unsigned int i, long long ut, long long et) :
    open(o), high(h), low(l), close(c), buy_volume(bv), volume(v), interval(i), unix_time(ut), event_time(et) {}

  double open;
  double high;
  double low;
  double close;
  double buy_volume;
  double volume;
  unsigned int interval;
  long long unix_time;
  long long event_time;
};
