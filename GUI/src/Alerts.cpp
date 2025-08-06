#include "Alerts.hpp"

Alerts::Alerts(SubscriptionManager& sm_) { 
  am = std::make_shared<AlertsManager>(sm_);
  window_name = "Alerts ##" + std::to_string(window_id);
}

void Alerts::init() {}

void Alerts::draw() {
  ImGui::SetNextWindowSize(ImVec2(750, 300), ImGuiCond_FirstUseEver);

  ImGui::Begin(window_name.c_str(), &show);
  
  ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
  ImGui::SetNextItemWidth(180.f);
  if (ImGui::BeginCombo("##Symbol", current_symbol.c_str())) {
    for (int i = 0; i < symbols.size(); i++) {
      bool is_selected = (current_symbol == symbols[i]);
      if (ImGui::Selectable(symbols[i].c_str(), is_selected)) {
        current_symbol = symbols[i];
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
  
  ImGui::SameLine();
  
  ImGui::SetNextItemWidth(180.f);
  if (ImGui::BeginCombo("##Direction", direction.c_str())) {
    for (int i = 0; i < directions.size(); i++) {
      bool is_selected = (direction == directions[i]);
      if (ImGui::Selectable(directions[i].c_str(), is_selected)) {
        direction = directions[i];
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
  
  ImGui::SameLine();

  ImGui::SetNextItemWidth(100.f);
  ImGui::InputDouble("##Price", &price);
  ImGui::SameLine();

  ImGui::SetNextItemWidth(80.f);
  if (ImGui::Button("Add Alert")) {
    Direction dir = (direction == "Above") ? Direction::ABOVE : Direction::BELOW;
    Alert alert(current_symbol, price, dir, genID());
    am->addAlert(alert);
  }

  ImGui::PopStyleVar();

  if (ImGui::BeginTable("alerts_table", 5, ImGuiTableFlags_SizingStretchSame)) {
    ImGui::TableSetupColumn("Symbol");
    ImGui::TableSetupColumn("Direction");
    ImGui::TableSetupColumn("Price");
    ImGui::TableSetupColumn("Status");
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();
    
    for (auto& [id, alert] : am->alerts) {
      ImVec4 color = Colors::White_V4;
      if (alert.triggered) color = Colors::Green_V4;
      
      if (alert.triggered && !alert.played) {
        playWav("cash_register.wav"); 
        alert.played = true;
        am->removeSubscription(id);
      }

      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::TextColored(color, "%s", alert.symbol.c_str());
      ImGui::TableNextColumn();
      ImGui::TextColored(color, "%s", directions[static_cast<size_t>(alert.direction)].c_str());
      ImGui::TableNextColumn();
      ImGui::TextColored(color, "%f.2", alert.price);
      ImGui::TableNextColumn();
      
      std::string status = (alert.triggered == true) ? "Triggered" : "Pending";
      ImGui::TextColored(color, "%s", status.c_str());
      ImGui::TableNextColumn();
      
      if (ImGui::Button(("Remove##" + std::to_string(id)).c_str())) {
        alerts_to_remove.push_back(id);
      }
    }

    ImGui::EndTable();
  } 
  ImGui::End();

  for (int64_t id : alerts_to_remove) {
    am->removeSubscription(id);
    am->removeAlert(id);
  }

  alerts_to_remove.clear();
}

uint64_t Alerts::genID() {
  std::mt19937_64 mt{ std::random_device{}() };
  std::uniform_int_distribution<int64_t> randomID{11111111, std::numeric_limits<int64_t>::max()};
  return randomID(mt);
}
