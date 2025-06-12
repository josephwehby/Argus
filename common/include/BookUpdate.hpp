#pragma once

#include "IEvent.hpp"

struct EntryUpdate {
  
  EntryUpdate() {}

  EntryUpdate(double p, double s) : price(p), size(s) {}

  double price;
  double size;
};

struct BookUpdate : public IEvent {
  std::string symbol;

  unsigned long long first_update;
  unsigned long long last_update;

  std::vector<EntryUpdate> bids;
  std::vector<EntryUpdate> asks;
};
