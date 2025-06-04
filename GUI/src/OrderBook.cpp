#include "OrderBook.hpp"

OrderBook::OrderBook(std::shared_ptr<WebSocket> _ws, std::string token) : ws(_ws) {
  symbol = token;
  window_name = "OrderBook: " + symbol + " ##" + std::to_string(window_id);
  
  //mobm->addOrderBook(symbol);
}

OrderBook::~OrderBook() {
  // need to add some sort of count to mobm so we can unsub if i am done reading from an ob
}

void OrderBook::updateBook() {
  //book = std::move(mobm->getOrderBook(symbol));
}

void OrderBook::draw() {

  updateBook();
  
  ImGui::SetNextWindowSize(ImVec2(435,1070), ImGuiCond_FirstUseEver);
  ImGui::Begin(window_name.c_str(), &show, ImGuiWindowFlags_NoScrollbar);

  ImVec2 pos = ImGui::GetWindowPos();
  ImVec2 win_size = ImGui::GetContentRegionAvail();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  
  float max_width = win_size.x;
  float min_width = max_width * .05;
  float x_start = win_size.x *.01;
  float y_start = win_size.y * .03;

  float bar_height = std::max((win_size.y) / 40.0f, min_bar_height);
  double max_bid_size = 0;
  double max_ask_size = 0;

  for (const auto& level : book.bids) max_bid_size = std::max(level.second, max_bid_size);
  for (const auto& level : book.asks) max_ask_size = std::max(level.second, max_ask_size);
  
  int row = 0;

  // draw ask side of orderbook
  for (const auto& level : book.asks) {

    float bar_length = static_cast<float>(level.second / max_ask_size) * max_width;
    bar_length = std::max(min_width, bar_length);

    float x1 = pos.x + x_start + (max_width - bar_length);
    float y1 = pos.y + y_start + (bar_height * row);

    float x2 = pos.x + x_start + max_width;
    float y2 = y1 + bar_height;

    draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), Colors::Red_Bar_U32);
    
    ImVec2 ask_price_pos(pos.x + x_start + 5, y1 + (bar_height/2) - 10);
    std::string ask_price = Utils::formatPrice(level.first); 
    draw_list->AddText(ask_price_pos, Colors::Red_U32, ask_price.c_str());

    std::string ask_size = std::to_string(level.second);
    ImVec2 ask_size_size = ImGui::CalcTextSize(ask_size.c_str());
    ImVec2 ask_size_pos(x2 - ask_size_size.x - 5, y1 + (bar_height/2) - 10); 

    draw_list->AddText(ask_size_pos, Colors::White_U32, ask_size.c_str());

    row++;
  }

  // draw the bid side of the order book
  for (const auto& level : book.bids) {

    float bar_length = static_cast<float>(level.second / max_bid_size) * max_width;
    bar_length = std::max(min_width, bar_length);

    float x1 = pos.x + x_start + (max_width - bar_length);
    float y1 = pos.y + y_start + (bar_height * row);

    float x2 = pos.x + x_start + max_width;
    float y2 = y1 + bar_height;

    draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), Colors::Green_Bar_U32);
    
    ImVec2 bid_price_pos(pos.x + x_start + 5, y1 + (bar_height/2) - 10);
    std::string bid_price = Utils::formatPrice(level.first); 
    draw_list->AddText(bid_price_pos, Colors::Green_U32, bid_price.c_str());

    std::string bid_size = std::to_string(level.second);
    ImVec2 bid_size_size = ImGui::CalcTextSize(bid_size.c_str());
    ImVec2 bid_size_pos(x2 - bid_size_size.x - 5, y1 + (bar_height/2) - 10); 

    draw_list->AddText(bid_size_pos, Colors::White_U32, bid_size.c_str());

    row++;
  }

  ImGui::End();
}
