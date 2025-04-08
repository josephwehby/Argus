#pragma once

struct Level1Data {
  
  Level1Data() {}

  Level1Data(double bb, double ba, double bbs, double bas, double price_c, double per_c, double lp) : 
    best_bid(bb), best_ask(ba), best_bid_size(bbs), best_ask_size(bas), price_change(price_c), percent_change(per_c), last_price(lp) {}

  double best_bid;
  double best_ask;
  double best_bid_size;
  double best_ask_size;
  double price_change;
  double percent_change;
  double last_price;
};
