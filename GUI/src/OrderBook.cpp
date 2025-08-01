#include "OrderBook.hpp"

OrderBook::OrderBook(SubscriptionManager& sm_, HttpsClient& hc_, std::string token) :  
  book_manager(std::make_shared<OrderBookManager>(sm_, hc_, token, window_id)) {

  symbol = token;
  window_name = "OrderBook: " + symbol + " ##" + std::to_string(window_id);
  book_manager->init();
}

OrderBook::~OrderBook() {
  book_manager->shutdown(); 
}

void OrderBook::init() {}

void OrderBook::draw() {
  
  book = book_manager->getBookSnapshot();
  
  ImGui::SetNextWindowSize(ImVec2(435,1070), ImGuiCond_FirstUseEver);
  ImGui::Begin(window_name.c_str(), &show, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("View")) {
      if (ImGui::MenuItem("Classic")) classic = true;
      if (ImGui::MenuItem("Depth")) classic = false;
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  if (classic) {
    drawClassic(draw_list);
  } else {
    drawDepth(draw_list);
  }
  ImGui::End();
}

void OrderBook::drawClassic(ImDrawList* draw_list) {
  ImVec2 pos = ImGui::GetWindowPos();
  ImVec2 win_size = ImGui::GetContentRegionAvail();
  
  float max_width = win_size.x;
  float min_width = max_width * .05;
  float x_start = win_size.x *.01;
  float y_start = win_size.y * .05;

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
}

void OrderBook::drawDepth(ImDrawList* draw_list) {
  std::vector<double> bid_prices(depth/bin_size), bid_volumes(depth/bin_size);
  std::vector<double> ask_prices(depth/bin_size), ask_volumes(depth/bin_size);
  
  int index = 0;
  for (auto it = book.bids.rbegin(); it != book.bids.rend(); ) {
    double volume = 0;
    for (int i = 0; i < bin_size && it != book.bids.rend(); i++, it++) {
      volume += it->second;
    }

    bid_prices[index] = index*bin_size;
    bid_volumes[index] = volume;
    index++;
  }
  
  index = 0;
  for (auto it = book.asks.rbegin(); it != book.asks.rend(); ) {
    double volume = 0;
    for (int i = 0; i < bin_size && it != book.asks.rend(); i++, it++) {
      volume += it->second;
    }
    ask_prices[index] = index*bin_size+depth;
    ask_volumes[index] = volume;
    index++;
  }

  double max_bid_size = 0;
  double max_ask_size = 0;
  
  for (const double volume : bid_volumes) max_bid_size = std::max(volume, max_bid_size);
  for (const double volume : ask_volumes) max_ask_size = std::max(volume, max_ask_size);
  
  for (int i = 0; i < bid_volumes.size(); i++) bid_volumes[i] = std::max(1.0, (bid_volumes[i]/max_bid_size)*max_height);
  for (int i = 0; i < ask_volumes.size(); i++) ask_volumes[i] = std::max(1.0, (ask_volumes[i]/max_ask_size)*max_height);
  
  bid_prices.push_back(bid_prices.back() + bin_size);
  bid_volumes.push_back(bid_volumes.back());
  
  ask_prices.push_back(ask_prices.back() + bin_size);
  ask_volumes.push_back(ask_volumes.back());

  /*
  int i = 0;
  for (auto it = book.bids.rbegin(); it != book.bids.rend(); it++, i++) {
    bid_prices[i] = i; 
    bid_volumes[i] = std::max((it->second/max_bid_size)*max_height, 1.0);
  }
  
  i = 0;
  for (auto it = book.asks.rbegin(); it != book.asks.rend(); it++, i++) {
    ask_prices[i] = i + depth; 
    ask_volumes[i] = std::max((it->second/max_ask_size)*max_height, 1.0);
  }
  */
  if (ImPlot::BeginPlot("##Market Depth")) {
    ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels);
    ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels);
    
    ImPlot::SetupAxisLimits(ImAxis_X1, 0, depth*2, ImPlotCond_Always);
    ImPlot::SetupAxisLimits(ImAxis_Y1, 0, max_height, ImPlotCond_Always);

    ImPlot::PushStyleColor(ImPlotCol_Fill, Colors::Green_Light_V4);
    ImPlot::PushStyleColor(ImPlotCol_Line, Colors::Green_Light_V4);
    ImPlot::PlotShaded("##Bids", bid_prices.data(), bid_volumes.data(), bid_volumes.size());
    ImPlot::PopStyleColor();
    ImPlot::PushStyleColor(ImPlotCol_Fill, Colors::Red_Light_V4);
    ImPlot::PushStyleColor(ImPlotCol_Line, Colors::Red_Light_V4);
    ImPlot::PlotShaded("##Asks", ask_prices.data(), ask_volumes.data(), ask_volumes.size());
    ImPlot::PopStyleColor();
    ImPlot::EndPlot();
  }
}
