#pragma once

#include "Widget.hpp"
#include "Level1.hpp"
#include "Utils.hpp"

class Ticker : public Widget {
  public:
    Ticker();
    void draw() override; 
  private:
    const std::string channel = "ticker";
    const float bar_length = 400;
    const float bar_height = 20;
    const float x_start = 5;
    const float y_start = 40;
    const float y_bars_start = 80;
    Level1Data level1;
};
