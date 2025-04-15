#include "Trades.hpp"

Trades::Trades(DataStore& ds, std::shared_ptr<WebSocket> _ws, std::string token) : datastore(ds), ws(_ws) {
  symbol = token;
  window_name = "Trades: " + symbol + " ##" + std::to_string(window_id);

  json sub_msg = JsonBuilder::generateSubscribe(symbol, channel);
  ws->subscribe(sub_msg);
}

Trades::~Trades() {
  json unsub_msg = JsonBuilder::generateUnsubscribe(symbol, channel);
  ws->unsubscribe(unsub_msg);
}

void Trades::draw() {
  
  auto update = datastore.getTrades(symbol);

  if (update.size() > 0) {
    for (auto& t : update) {
      trades.push_front(std::move(t));
      if (trades.size() > max_capacity) trades.pop_back();
    }
  }

  ImGui::SetNextWindowSize(ImVec2(280, 690), ImGuiCond_Always);
  ImGui::Begin(window_name.c_str(), &show);
  
  for (const auto& trade : trades) {
    ImVec4 color;
    if (trade.side == TradeSide::Buy) {
      color = (trade.type == TradeType::Limit) ? limitBuyColor : marketBuyColor;
    } else {
      color = (trade.type == TradeType::Limit) ? limitSellColor : marketSellColor;
    }

    ImGui::TextColored(color, "%-10s %-10s %-6s", trade.price.c_str(), trade.size.c_str(), trade.time.c_str());
  }


  ImGui::End();
}
