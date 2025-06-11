#include "Ticker.hpp"

Ticker::Ticker(std::shared_ptr<WebSocket> ws_, std::shared_ptr<EventBus> eb_, std::string token) : ws(ws_), eb(eb_) {
  symbol = token;
  window_name = "Ticker: " + symbol + "  ##" + std::to_string(getWindowID());
  event_channel = "LEVEL1:" + symbol;
  
  json sub_msg = JsonBuilder::generateSubscribe(symbol, channel, window_id); 
  ws->subscribe(sub_msg);
  
  eb->subscribe(event_channel, window_id, [this](const std::shared_ptr<IEvent> update) {
    auto level1_event = std::dynamic_pointer_cast<Level1>(update);
    if (level1_event->empty) return;
    this->level1 = *level1_event;
  });

}

Ticker::~Ticker() {
  json unsub_msg = JsonBuilder::generateUnsubscribe(symbol, channel, window_id);
  ws->unsubscribe(unsub_msg);
  eb->unsubscribe(event_channel, window_id);
}

void Ticker::draw() {
  
  if (level1.empty) return;
  
  ImGui::SetNextWindowSize(ImVec2(440, 250), ImGuiCond_FirstUseEver);
  
  ImGui::Begin(window_name.c_str(), &show);
  
  ImGui::Dummy(ImVec2(0.f, 5.f));
  
  ImVec4 text_color = (level1.price_change >= 0) ? Colors::Green_V4 : Colors::Red_V4;

  ImGui::PushFont(App::large_font);
  ImGui::TextColored(text_color, "%.2f", level1.last_price);
  ImGui::PopFont();
  ImGui::SameLine(); 
  ImGui::TextColored(text_color, "%+.2f", level1.price_change);
  
  ImGui::Dummy(ImVec2(0.f, 10.f));

  ImGui::Text("%-10s %.2f", "High:", level1.high);
  ImGui::Text("%-10s %.2f", "Low:", level1.low);
  ImGui::Text("%-10s %.2f", "Volume:", level1.volume);

  ImGui::Dummy(ImVec2(0.f, 10.f));
  
  ImGui::BeginChild("BidAskBar", ImVec2(0, bar_height*2 + 10), false, ImGuiWindowFlags_NoScrollbar);
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  
  ImVec2 pos = ImGui::GetWindowPos();
  ImVec2 win_size = ImGui::GetContentRegionAvail();

  float total_size = level1.best_bid_size + level1.best_ask_size;
  float bar_length = win_size.x;
  float y_start = win_size.y * .01;

  float x1 = pos.x;
  float y1 = y_start + pos.y; 
  float x2 = x1 + (level1.best_bid_size / total_size * bar_length);
  float y2 = y1 + bar_height;

  draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), Colors::Green_Bar_U32);

  x1 = x2;
  x2 = pos.x + bar_length;
  draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), Colors::Red_Bar_U32);
  
  x1 = pos.x;
  y1 = y_start + pos.y + bar_height; 
  x2 = x1 + (bar_length/2);
  y2 = y1 + bar_height;

  draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), Colors::Green_Light_U32);

  x1 = x2;
  x2 = pos.x + bar_length;
  draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), Colors::Red_Light_U32);
   
  // add bid size to bar and price to bar
  float y_pos = pos.y + y_start + bar_height + (bar_height)/2 - 10;

  ImVec2 bid_size_pos(pos.x + 3, y_pos); 
  std::string bid_size_text = std::to_string(level1.best_bid_size);
  draw_list->AddText(bid_size_pos, Colors::White_U32, bid_size_text.c_str()); 

  // best bid price
  std::string bid_price_text = Utils::formatPrice(level1.best_bid);
  ImVec2 bid_price_size = ImGui::CalcTextSize(bid_price_text.c_str());
  ImVec2 bid_price_pos(pos.x + bar_length/2 - bid_price_size.x - 5, y_pos);
  draw_list->AddText(bid_price_pos, Colors::Green_U32, bid_price_text.c_str());

  // best ask price
  std::string ask_price_text = Utils::formatPrice(level1.best_ask);
  ImVec2 ask_price_pos(pos.x + bar_length/2 + 5, y_pos);
  draw_list->AddText(ask_price_pos, Colors::Red_U32, ask_price_text.c_str());

  // best ask size
  std::string ask_size_text = std::to_string(level1.best_ask_size);
  ImVec2 ask_size_size = ImGui::CalcTextSize(ask_size_text.c_str());
  ImVec2 ask_size_pos(pos.x + bar_length - ask_size_size.x - 3, y_pos);
  draw_list->AddText(ask_size_pos, Colors::White_U32, ask_size_text.c_str());
  ImGui::EndChild();
   
  ImGui::End();
}
