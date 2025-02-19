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
  ImGui::Begin(window_name.c_str());
  ImPlot::BeginPlot("Candlestick Chart");

  ImDrawList* draw_list = ImPlot::GetPlotDrawList();


  
  ImPlot::EndPlot();
  ImGui::End();
}
