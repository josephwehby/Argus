#include "Chart.hpp"

Chart::Chart() {
  window_name = "Chart ##" + std::to_string(getWindowID()); 
  
  candles = {
    {97.5, 99.2, 97.1, 98.7, 1200.5, 1, "2025-02-19T09:00:00"},
    {98.7, 99.5, 98.2, 99.1, 1500.8, 1, "2025-02-19T09:00:01"},
    {99.1, 100.0, 98.9, 99.6, 1800.3, 1, "2025-02-19T09:00:02"},
    {99.6, 99.9, 98.8, 99.0, 1600.7, 1, "2025-02-19T09:00:03"},
    {99.0, 99.3, 97.9, 98.5, 1400.2, 1, "2025-02-19T09:00:04"},
    {98.5, 98.9, 97.7, 98.0, 1300.6, 1, "2025-02-19T09:00:05"},
    {98.0, 98.6, 97.5, 97.9, 1100.9, 1, "2025-02-19T09:00:06"},
    {97.9, 98.4, 97.6, 98.2, 1050.4, 1, "2025-02-19T09:00:07"},
    {98.2, 99.0, 98.0, 98.8, 1700.1, 1, "2025-02-19T09:00:08"},
    {98.8, 99.7, 98.4, 99.5, 1900.5, 1, "2025-02-19T09:00:09"}
  };

}

Chart::~Chart() {}

void Chart::draw() {
  ImGui::Begin(window_name.c_str(), &show);
  ImPlot::BeginPlot("Graph");
  
  ImDrawList* draw_list = ImPlot::GetPlotDrawList();

  double width_percent = .25;

  double half_width = candles.size() > 1 ? (Utils::UTCToUnix(candles[1].begin) - Utils::UTCToUnix(candles[0].begin)) * width_percent : width_percent;
  
  
  for (int i = 0; i < candles.size(); i++) {
    auto unix_time = Utils::UTCToUnix(candles[i].begin);

    ImVec2 open_pos = ImPlot::PlotToPixels(unix_time - half_width, candles[i].open);
    ImVec2 close_pos = ImPlot::PlotToPixels(unix_time + half_width, candles[i].close);
    ImVec2 low_pos = ImPlot::PlotToPixels(unix_time, candles[i].low);
    ImVec2 high_pos = ImPlot::PlotToPixels(unix_time, candles[i].high);
    ImU32 color = ImGui::GetColorU32(candles[i].open > candles[i].close ? green : red);
    draw_list->AddLine(low_pos, high_pos, color);
    draw_list->AddRectFilled(open_pos, close_pos, color);
  }


  ImPlot::EndPlot();
  
  ImGui::End();
}
