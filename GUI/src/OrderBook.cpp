#include "OrderBook.hpp"

OrderBook::OrderBook(DataStore& ds, std::shared_ptr<WebSocket> _ws, std::string token) : datastore(ds), ws(_ws) {
  symbol = token;
  window_name = "OrderBook: " + symbol + " ##" + std::to_string(window_id);

  json sub_msg = JsonBuilder::generateSubscribe(symbol, channel);
  ws->subscribe(sub_msg);
}

OrderBook::~OrderBook() {
  json unsub_msg = JsonBuilder::generateUnsubscribe(symbol, channel);
  ws->unsubscribe(unsub_msg);
}

void OrderBook::updateBook() {
  auto updates = datastore.getBook(symbol);

  for (auto bid : updates.bids) {
    if (bid.size == 0) {
      bids.erase(bid.price); 
    } else {
      bids[bid.price] = bid.size; 
    }
  }

  for (auto ask : updates.asks) {
    if (ask.size == 0) {
      asks.erase(ask.price); 
    } else {
      asks[ask.price] = ask.size; 
    }
  }

  while (bids.size() > depth) {
    auto it = std::prev(bids.end());
    bids.erase(it);
  }

  while (asks.size() > depth) {
    auto it = std::prev(asks.end());
    asks.erase(it);
  }

}

void OrderBook::draw() {

  updateBook();

  ImGui::SetNextWindowSize(ImVec2(435,475), ImGuiCond_Always);
  ImGui::Begin(window_name.c_str(), &show);

  ImVec2 pos = ImGui::GetWindowPos();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  
  double max_bid_size = 0;
  double max_ask_size = 0;

  for (const auto& level : bids) max_bid_size = std::max(level.second, max_bid_size);
  for (const auto& level : asks) max_ask_size = std::max(level.second, max_ask_size);
  
  int row = 0;

  // draw ask side of orderbook
  for (const auto& level : asks) {
    
    float bar_length = static_cast<float>(level.second / max_ask_size) * max_width;
    bar_length = std::max(min_width, bar_length);

    float x1 = pos.x + x_start + (max_width - bar_length);
    float y1 = pos.y + y_start + ((bar_height+2) * row);

    float x2 = pos.x + x_start + max_width;
    float y2 = y1 + bar_height;

    draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(230, 0, 0, 100));
    
    ImVec2 ask_price_pos(pos.x + x_start + 5, y1 + (bar_height/2) - 10);
    std::string ask_price = Utils::formatPrice(level.first); 
    draw_list->AddText(ask_price_pos, IM_COL32(255, 0, 0, 255), ask_price.c_str());

    std::string ask_size = std::to_string(level.second);
    ImVec2 ask_size_size = ImGui::CalcTextSize(ask_size.c_str());
    ImVec2 ask_size_pos(x2 - ask_size_size.x - 5, y1 + (bar_height/2) - 10); 

    draw_list->AddText(ask_size_pos, IM_COL32(255,255,255,255), ask_size.c_str());

    row++;
  }

  // draw the bid side of the order book
  for (const auto& level : bids) {
    
    float bar_length = static_cast<float>(level.second / max_bid_size) * max_width;
    bar_length = std::max(min_width, bar_length);

    float x1 = pos.x + x_start + (max_width - bar_length);
    float y1 = pos.y + y_start + ((bar_height+2) * row);

    float x2 = pos.x + x_start + max_width;
    float y2 = y1 + bar_height;

    draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(0, 200, 5, 100));
    
    ImVec2 bid_price_pos(pos.x + x_start + 5, y1 + (bar_height/2) - 10);
    std::string bid_price = Utils::formatPrice(level.first); 
    draw_list->AddText(bid_price_pos, IM_COL32(0, 255, 0, 255), bid_price.c_str());

    std::string bid_size = std::to_string(level.second);
    ImVec2 bid_size_size = ImGui::CalcTextSize(bid_size.c_str());
    ImVec2 bid_size_pos(x2 - bid_size_size.x - 5, y1 + (bar_height/2) - 10); 

    draw_list->AddText(bid_size_pos, IM_COL32(255,255,255,255), bid_size.c_str());

    row++;
  }

  ImGui::End();
}
