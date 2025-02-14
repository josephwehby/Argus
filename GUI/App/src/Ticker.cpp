#include "Ticker.hpp"

Ticker::Ticker() {
  window_name = "Ticker ##" + std::to_string(getWindowID());

  level1 = {
    99.75,
    100.05,
    300,
    150,
    -1.234,
    -.17,
    99.98
  };
}

void Ticker::draw() {
  ImGui::SetNextWindowSize(ImVec2(410, 150), ImGuiCond_Always);
  ImGui::Begin(window_name.c_str(), &show);
  ImVec2 pos = ImGui::GetWindowPos();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  float x1, x2, y1, y2;
  int total_size = level1.best_bid_size + level1.best_ask_size;
  

  // draw the bar the shows the order size
  x1 = pos.x + x_start;
  y1 = pos.y + y_start;

  x2 = x1 + (level1.best_bid_size / total_size * bar_length);
  y2 = y1 + bar_height - 7;

  draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(0, 200, 5, 180));
  
  x1 = x2;
  x2 = pos.x + x_start + bar_length;
  draw_list->AddRectFilled(ImVec2(x1,y1), ImVec2(x2, y2), IM_COL32(230, 0, 0, 180));
  

  // draw the price, size and bars
  x1 = pos.x + x_start;
  y1 = pos.y + y_start + bar_height - 7;
  x2 = x1 + bar_length/2;
  y2 = y1 + bar_height;

  draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(0, 200, 5, 100));

  x1 = x2;
  x2 = pos.x + x_start + bar_length;
  draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(230, 0, 0, 100));

  // add best size and best price
  float y_pos = pos.y + y_start + bar_height - 7 + (bar_height)/2 - 10;

  ImVec2 bid_size_pos(pos.x + x_start + 3, y_pos);
  std::string bid_size_text = std::to_string(static_cast<int>(level1.best_bid_size));
  draw_list->AddText(bid_size_pos, IM_COL32(255, 255, 255, 255), bid_size_text.c_str()); 
  
  std::string bid_price_text = Utils::formatPrice(level1.best_bid);
  ImVec2 bid_price_size = ImGui::CalcTextSize(bid_size_text.c_str());
  ImVec2 bid_price_pos(pos.x + x_start + bar_length/2 - bid_price_size.x - 5, y_pos);
  draw_list->AddText(bid_price_pos, IM_COL32(0, 255, 0, 255), bid_price_text.c_str()); 

  ImGui::End();
}
