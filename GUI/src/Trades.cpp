#include "Trades.hpp"

Trades::Trades(SubscriptionManager& sm_, std::string token) : sm(sm_) {
  symbol = token;
  window_name = "Trades: " + symbol + " ##" + std::to_string(window_id);
  event_channel = "TRADE:" + symbol;
}

Trades::~Trades() {
  sm.unsubscribe(request);
}

void Trades::init() {

  std::weak_ptr<Trades> self = shared_from_this();

  request = SubscriptionRequest { 
    symbol, 
    channel, 
    event_channel, 
    window_id, 
    [self] (const std::shared_ptr<IEvent> update) {
      if (auto locked = self.lock()) {
        auto trade_event = std::dynamic_pointer_cast<Trade>(update); 
        if (std::stod(trade_event->size) >= locked->filter) locked->trades.push_front(*trade_event);
        if (locked->trades.size() > locked->max_capacity) locked->trades.pop_back();
      }
    },
    std::nullopt,
    false
  };
  
  sm.subscribe(request);

}

void Trades::draw() {
   
  ImGui::SetNextWindowSize(ImVec2(370, 690), ImGuiCond_FirstUseEver);
  ImGui::Begin(window_name.c_str(), &show);

  ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
  if(ImGui::InputDouble("Qty", &filter)) {
    trades.clear();
  };
  ImGui::SameLine(0, 15.f);
  if (ImGui::Button("Reset")) {
    trades.clear();
    filter = 0;
  }

  ImGui::PopStyleVar();

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
