#pragma once

#include "App.hpp"
#include "Widget.hpp"
#include "Level1.hpp"
#include "Utils.hpp"
#include "DataStore.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"

class Ticker : public Widget {
  public:
    Ticker(std::shared_ptr<DataStore>, std::shared_ptr<WebSocket>, std::string);
    ~Ticker();
    void draw() override; 
  private:
    const std::string channel = "ticker";
    
    const float bar_height = 25;
    
    const ImVec4 ask_text_color = ImVec4{1.f, 102/255.f, 102/255.f, 1.f};
    const ImVec4 bid_text_color = ImVec4{102/255.f, 1.f, 153/255.f, 1.f};
    
    const ImU32 ask_text_color_32 = IM_COL32(255, 102, 102, 255);
    const ImU32 bid_text_color_32 = IM_COL32(102, 255, 153, 255);
    const ImU32 white_32 = IM_COL32(255, 255, 255, 255);
    const ImU32 ask_bar_color = IM_COL32(255, 64, 64, 153);
    const ImU32 bid_bar_color = IM_COL32(51, 204, 102, 153);
    const ImU32 ask_bar_color_light = IM_COL32(255, 64, 64, 90);
    const ImU32 bid_bar_color_light = IM_COL32(51, 204, 102, 90);

    std::shared_ptr<Level1> level1;
    std::shared_ptr<DataStore> datastore;
    std::shared_ptr<WebSocket> ws;
};
