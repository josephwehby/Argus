#pragma once
// potentially add another 2 fields where the data is string represented so that we dont have to keep calling
// string operations every time orderbook renders

struct EntryUpdate {
  
  EntryUpdate() {}

  EntryUpdate(double p, double s) : price(p), size(s) {}

  double price;
  double size;
};

struct BookUpdate {
  std::string symbol;

  unsigned long first_update;
  unsigned long last_update;

  std::vector<EntryUpdate> bids;
  std::vector<EntryUpdate> asks;
};
