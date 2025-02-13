#include "Trades.hpp"

Trades::Trades() {
  window_name = "Trades " + std::to_string(window_id);
}

Trades::~Trades() {

}

void Trades::draw() {
  ImGui::SetNextWindowSize(ImVec2(210, 500), ImGuiCond_Always);
  ImGui::Begin(window_name.c_str(), &show);
  
  for (const auto& trade : trades) {
    
    std::string size = std::to_string(trade.size);
    std::string price = Utils::formatPrice(trade.price);
    std::string time = Utils::formatTime(trade.time);

    if (trade.type == TradeType::Buy) {
      ImGui::TextColored({0.f, 200/255.f, 5/255.f, 255/255.f}, "%-5s %-8s %-8s", size.c_str(), price.c_str(), time.c_str());  
    } else {
      ImGui::TextColored({230/255.f, 0.f, 0.f, 255/255.f}, "%-5s %-8s %-8s", size.c_str(), price.c_str(), time.c_str()); 
    }
  }

  ImGui::End();
}
