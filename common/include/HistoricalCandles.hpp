#pragma once

#include <vector>

#include "IEvent.hpp"
#include "Candle.hpp"

struct HistoricalCandles : public IEvent {
  std::vector<Candle> candles;
};
