#include "OrderBook.hpp"

OrderBook::OrderBook(std::string token) {
  symbol = token;
  window_name = "OrderBook: " + symbol + " ##" + std::to_string(window_id);
}

OrderBook::~OrderBook() {}

void OrderBook::readData() {}

void OrderBook::draw() {
  ImGui::SetNextWindowSize(ImVec2(435,255), ImGuiCond_Always);
  ImGui::Begin(window_name.c_str(), &show);

  ImVec2 pos = ImGui::GetWindowPos();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  
  double max_bid_size = 0;
  double max_ask_size = 0;

  for (const auto& level : bids) max_bid_size = std::max(level.second, max_bid_size);
  for (const auto& level : asks) max_ask_size = std::max(level.second, max_ask_size);
  
  // draw the bid side of the order book
  int row = 0;
  for (const auto& level : bids) {

    float length = static_cast<float>(level.second / max_bid_size) * max_width;

    float y_offset = pos.y + 30 + ((bar_height + 2)*row);
    float x = (pos.x + x_start + max_width) - length;

    draw_list->AddRectFilled(ImVec2(x, y_offset), 
        ImVec2(pos.x + x_start + max_width, y_offset + bar_height), 
        IM_COL32(0, 200, 5, 100));
    
    std::string bid_price = Utils::formatPrice(level.first);
    ImVec2 bid_text_size = ImGui::CalcTextSize(bid_price.c_str());
    ImVec2 bid_price_pos((pos.x + x_start + max_width) - bid_text_size.x - 5, y_offset + (bar_height/2) - 10);

    draw_list->AddText(bid_price_pos, IM_COL32(0, 255, 0, 255), bid_price.c_str());
    
    std::string bid_size = std::to_string(static_cast<int>(level.second));
    ImVec2 bid_size_pos(pos.x + x_start + 5, y_offset + (bar_height/2) - 10); 

    draw_list->AddText(bid_size_pos, IM_COL32(255,255,255,255), bid_size.c_str());
    row++;
  }
  
  // draw the ask side of the orderbook
  row = 0;
  for (const auto& level : asks) {
    
    float length = static_cast<float>(level.second / max_ask_size) * max_width;

    float y_offset = pos.y + 30 + ((bar_height + 2)*row);
    float x_offset = pos.x + x_start + max_width + 5;
     
    draw_list->AddRectFilled(ImVec2(x_offset, y_offset), 
        ImVec2(x_offset + length, y_offset + bar_height), 
        IM_COL32(230, 0, 0, 100));

    ImVec2 ask_price_pos(x_offset + 5, y_offset + (bar_height/2) - 10);
    std::string ask_price = Utils::formatPrice(level.first); 
    draw_list->AddText(ask_price_pos, IM_COL32(255, 0, 0, 255), ask_price.c_str());

    std::string ask_size = std::to_string(static_cast<int>(level.second));
    ImVec2 ask_size_size = ImGui::CalcTextSize(ask_size.c_str());
    ImVec2 ask_size_pos(x_offset + max_width - ask_size_size.x - 5, y_offset + (bar_height/2) - 10); 

    draw_list->AddText(ask_size_pos, IM_COL32(255,255,255,255), ask_size.c_str());

    row++;
  }

  ImGui::End();
}
