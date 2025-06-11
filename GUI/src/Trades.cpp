#include "Trades.hpp"

Trades::Trades(std::shared_ptr<WebSocket> ws_, std::shared_ptr<EventBus> eb_, std::string token) : ws(ws_), eb(eb_) {
  symbol = token;
  window_name = "Trades: " + symbol + " ##" + std::to_string(window_id);
  event_channel = "TRADE:" + symbol;
  
  json sub_msg = JsonBuilder::generateSubscribe(symbol, channel, window_id);
  ws->subscribe(sub_msg);

  eb->subscribe(event_channel, window_id, [this](const std::shared_ptr<IEvent> update) {
    auto trade_event = std::dynamic_pointer_cast<Trade>(update); 
    this->trades.push_front(*trade_event);
    if (this->trades.size() > this->max_capacity) this->trades.pop_back();
  });
}

Trades::~Trades() {
  json unsub_msg = JsonBuilder::generateUnsubscribe(symbol, channel, window_id);
  ws->unsubscribe(unsub_msg);
  eb->unsubscribe(event_channel, window_id);
}

void Trades::draw() {
   
  ImGui::SetNextWindowSize(ImVec2(340, 690), ImGuiCond_FirstUseEver);
  ImGui::Begin(window_name.c_str(), &show);

  if (ImGui::BeginTable("trades_table", 3, ImGuiTableFlags_SizingStretchSame)) {
    ImGui::TableSetupColumn("Price");
    ImGui::TableSetupColumn("Qty");
    ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    for (const auto& trade : trades) {
      ImGui::TableNextRow();
      
      ImVec4 color;
      
      if (trade.side == TradeSide::Buy) {
        color = Colors::Green_V4; 
      } else {
        color = Colors::Red_V4;
      }
    
      ImGui::TableNextColumn();
      ImGui::TextColored(color, "%s", trade.price.c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%s", trade.size.c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%s", trade.time.c_str());
    }

    ImGui::EndTable();
  } 

  ImGui::End();
}
