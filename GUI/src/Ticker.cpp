#include "Ticker.hpp"

Ticker::Ticker(DataStore& ds, std::shared_ptr<WebSocket> _ws, std::string token) : datastore(ds) {
  ws = _ws;
  symbol = token;
  window_name = "Ticker: " + symbol + "  ##" + std::to_string(getWindowID());
  
  json sub_msg = JsonBuilder::generateSubscribe("BTC/USD", "ticker"); 
  ws.subscribe(sub_msg);
}

// probably need to redo this at some point
void Ticker::draw() {
  
  // get updates from datastore if there are any
  auto update = datastore.getTicker(symbol);  

  if (update != nullptr) level1 = update;
  if (level1 == nullptr) return;
  
  ImGui::SetNextWindowSize(ImVec2(435, 100), ImGuiCond_Always);
  ImGui::Begin(window_name.c_str(), &show);
  ImVec2 pos = ImGui::GetWindowPos();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  
  draw_list->AddText({x_start + pos.x, y_start + pos.y}, IM_COL32(255,255,255,255), "BID");
  
  // draw last price sale took place at
  float mid_point = pos.x + x_start + bar_length/2;
  std::string last_price = Utils::formatPrice(level1->last_price);
  ImVec2 last_price_size = ImGui::CalcTextSize(last_price.c_str());

  mid_point -= (last_price_size.x / 2);
  
  if (level1->price_change >= 0) {
    draw_list->AddText({mid_point, pos.y + y_start}, IM_COL32(0, 255, 0, 255), last_price.c_str());
  } else {
    draw_list->AddText({mid_point, pos.y + y_start}, IM_COL32(255, 0, 0, 255), last_price.c_str());
  }

  ImVec2 text_size_ask = ImGui::CalcTextSize("ASK");
  draw_list->AddText({pos.x + x_start + bar_length - text_size_ask.x, y_start + pos.y}, IM_COL32(255,255,255,255), "ASK");

  float x1, x2, y1, y2;
  int total_size = level1->best_bid_size + level1->best_ask_size;

  // draw the bar the shows the order size
  x1 = pos.x + x_start;
  y1 = pos.y + y_bars_start;

  x2 = x1 + (level1->best_bid_size / total_size * bar_length);
  y2 = y1 + bar_height - 7;

  draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(0, 200, 5, 180));
  
  x1 = x2;
  x2 = pos.x + x_start + bar_length;
  draw_list->AddRectFilled(ImVec2(x1,y1), ImVec2(x2, y2), IM_COL32(230, 0, 0, 180));
  
  // draw the price, size and bars
  x1 = pos.x + x_start;
  y1 = pos.y + y_bars_start + bar_height - 7;
  x2 = x1 + bar_length/2;
  y2 = y1 + bar_height;

  draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(0, 200, 5, 100));

  x1 = x2;
  x2 = pos.x + x_start + bar_length;
  draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(230, 0, 0, 100));
  
  float y_pos = pos.y + y_bars_start + bar_height - 7 + (bar_height)/2 - 10;
  
  // best bid size
  ImVec2 bid_size_pos(pos.x + x_start + 3, y_pos);
  std::string bid_size_text = std::to_string(static_cast<int>(level1->best_bid_size));
  draw_list->AddText(bid_size_pos, IM_COL32(255, 255, 255, 255), bid_size_text.c_str()); 
  
  // best bid price
  std::string bid_price_text = Utils::formatPrice(level1->best_bid);
  ImVec2 bid_price_size = ImGui::CalcTextSize(bid_price_text.c_str());
  ImVec2 bid_price_pos(pos.x + x_start + bar_length/2 - bid_price_size.x - 5, y_pos);
  draw_list->AddText(bid_price_pos, IM_COL32(0, 255, 0, 255), bid_price_text.c_str()); 
  
  // best ask price
  std::string ask_price_text = Utils::formatPrice(level1->best_ask);
  ImVec2 ask_price_pos(pos.x + x_start + bar_length/2 + 5, y_pos);
  draw_list->AddText(ask_price_pos, IM_COL32(255, 0, 0, 255), ask_price_text.c_str()); 

  // best ask size
  std::string ask_size_text = std::to_string(static_cast<int>(level1->best_ask_size));
  ImVec2 ask_size_size = ImGui::CalcTextSize(ask_size_text.c_str());
  ImVec2 ask_size_pos(pos.x + x_start + bar_length - ask_size_size.x - 3, y_pos);
  draw_list->AddText(ask_size_pos, IM_COL32(255, 255, 255, 255), ask_size_text.c_str()); 
  ImGui::End();
}
