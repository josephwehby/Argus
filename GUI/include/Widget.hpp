#pragma once

#include <string>
#include <random>
#include <chrono>

#include "imgui.h"

class Widget {
  public:
    Widget() {
      std::mt19937 mt{ std::random_device{}() };
      std::uniform_int_distribution<> randomID{11111111,99999999};
      window_id = randomID(mt); 
    }

    int getWindowID() const { return window_id; }
    bool isOpen() { return show; }

    virtual ~Widget() = default;
    virtual void draw() = 0;

  protected:
    int window_id;
    bool show = true;
    std::string symbol;
    std::string window_name;
};
