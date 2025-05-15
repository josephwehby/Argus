#pragma once

#include <string>
#include <random>
#include <chrono>
#include <cstdint>
#include <limits>

#include "imgui.h"

class Widget {
  public:
    Widget() {
      std::mt19937_64 mt{ std::random_device{}() };
      std::uniform_int_distribution<int64_t> randomID{11111111, std::numeric_limits<int64_t>::max()};
      window_id = randomID(mt); 
    }

    int getWindowID() const { return window_id; }
    bool isOpen() { return show; }

    virtual ~Widget() = default;
    virtual void draw() = 0;

  protected:
    int64_t window_id;
    bool show = true;
    std::string symbol;
    std::string window_name;
};
