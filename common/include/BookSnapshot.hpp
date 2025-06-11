#pragma once

#include <map>

#include "IEvent.hpp"

struct BookSnapshot : public IEvent {
  unsigned long long last_update;

  std::map<double, double, std::greater<double>> bids;
  std::map<double, double, std::greater<double>> asks;
};
