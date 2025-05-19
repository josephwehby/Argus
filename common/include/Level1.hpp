#pragma once

struct Level1 {
  
  Level1() {
    empty = true;
  }

  Level1(double bb, double ba, double bbs, double bas, double price_c, double per_c, double lp, double h, double l, double v) : 
    best_bid(bb), best_ask(ba), best_bid_size(bbs), best_ask_size(bas), price_change(price_c), percent_change(per_c), last_price(lp), high(h), low(l), volume(v) { empty = false; }

  double best_bid;
  double best_ask;
  double best_bid_size;
  double best_ask_size;
  double price_change;
  double percent_change;
  double last_price;
  double high;
  double low;
  double volume;
  bool empty;
};
