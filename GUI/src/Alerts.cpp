#include "Alerts.hpp"

Alerts::Alerts(SubscriptionManager& sm_) : sm(sm_) { 
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
    // create new alert
  }
  ImGui::PopStyleVar();

  if (ImGui::BeginTable("alerts_table", 4, ImGuiTableFlags_SizingStretchSame)) {
    ImGui::TableSetupColumn("Symbol");
    ImGui::TableSetupColumn("Direction");
    ImGui::TableSetupColumn("Price");
    ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("BTC");
    ImGui::TableNextColumn();
    ImGui::Text("Above");
    ImGui::TableNextColumn();
    ImGui::Text("100,000.89");
    ImGui::TableNextColumn();
    ImGui::Text("Pending");

    ImGui::EndTable();
  } 
  ImGui::End();
}
