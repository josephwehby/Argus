#pragma once

#include <map>

struct BookSnapshot {
  unsigned long long last_update;

  std::map<double, double, std::greater<double>> bids;
  std::map<double, double, std::greater<double>> asks;
};
