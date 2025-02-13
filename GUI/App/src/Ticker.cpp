#include "Ticker.hpp"

Ticker::Ticker() {
  symbol = "BTC/USD";
  window_name = symbol + std::to_string(getWindowID());
}

void Ticker::draw() {
  ImGui::Begin(window_name.c_str(), &show);
  ImGui::Text("Price: $%lf", price); 
  ImGui::End();
}
