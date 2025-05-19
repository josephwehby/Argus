#include "Trades.hpp"

Trades::Trades(std::shared_ptr<DataStore> ds, std::shared_ptr<WebSocket> _ws, std::string token) : datastore(ds), ws(_ws) {
  symbol = token;
  window_name = "Trades: " + symbol + " ##" + std::to_string(window_id);

  json sub_msg = JsonBuilder::generateSubscribe(symbol, channel, window_id);
  ws->subscribe(sub_msg);
}

Trades::~Trades() {
  json unsub_msg = JsonBuilder::generateUnsubscribe(symbol, channel, window_id);
  ws->unsubscribe(unsub_msg);
}

void Trades::draw() {
   
  auto update = datastore->getTrades(symbol);
  
  if (update.size() > 0) {
    for (auto& t : update) {
      trades.push_front(std::move(t));
      if (trades.size() > max_capacity) trades.pop_back();
    }
  }

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
