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
    
    std::string size = std::to_string(trade.size);
    std::string price = Utils::formatPrice(trade.price);
    
    if (trade.type == TradeType::Limit) {
      if (trade.side == TradeSide::Buy) {
        ImGui::TextColored({0.f, 200/255.f, 5/255.f, 1.f}, "%-8s %-12s %-9s", size.c_str(), price.c_str(), trade.time.c_str());  
      } else {
        ImGui::TextColored({230/255.f, 0.f, 0.f, 1.f}, "%-8s %-12s %-9s", size.c_str(), price.c_str(), trade.time.c_str()); 
      }
    } else {
      ImGui::TextColored({0.f, 1.f, 1.f, 1.f}, "%-8s %-12s %-9s", size.c_str(), price.c_str(), trade.time.c_str());  
    }

  }

  ImGui::End();
}
