#pragma once

#include <cstdint>
#include <string>

enum class Direction { ABOVE, BELOW };

struct Alert {
  
  Alert() { triggered = false; played = false; } 
  
  Alert(std::string s, double p, Direction d, uint64_t i) : 
    symbol(s), price(p), direction(d), id(i), triggered(false), played(false) {}
  
  std::string symbol;
  double price;
  Direction direction;
  int64_t id;
  bool triggered;
  bool played;
};
