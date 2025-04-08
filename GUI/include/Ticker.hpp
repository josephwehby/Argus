#pragma once

#include "Widget.hpp"
#include "Level1.hpp"
#include "Utils.hpp"

#include "DataStore.hpp"

class Ticker : public Widget {
  public:
    Ticker(DataStore&, std::string);
    void draw() override; 
  private:
    const std::string channel = "ticker";
    const float bar_length = 400;
    const float bar_height = 20;
    const float x_start = 17.5;
    const float y_start = 30;
    const float y_bars_start = 50;
    Level1 level1;

    DataStore& datastore;
};
