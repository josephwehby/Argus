#include "Chart.hpp"
#include <ctime>
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

int xAxisFormatter(double value, char* buffer, int size, void*) {
  std::time_t t = static_cast<std::time_t>(value);
  std::tm* time_info = gmtime(&t);
  std::strftime(buffer, size, "%H:%M", time_info);
  return 0;
}

void Chart::draw() {
  ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
  ImGui::Begin(window_name.c_str(), &show);
  const double half_width = .25;

  if(ImPlot::BeginPlot("Graph", ImVec2(-1,-1), ImPlotFlags_NoLegend)) {

    ImDrawList* draw_list = ImPlot::GetPlotDrawList();
    ImPlot::SetupAxes(0,0,0,ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit|ImPlotAxisFlags_Opposite);
    ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time);
    ImPlot::SetupAxisLimits(ImAxis_X1, Utils::UTCToUnix(candles[0].begin), Utils::UTCToUnix(candles.back().begin));
    ImPlot::SetupAxisFormat(ImAxis_Y1, "$%.0f");

    ImPlot::SetupAxisFormat(ImAxis_X1, xAxisFormatter);
    
    std::string name = "Stocks ##" + window_name;

    if (ImPlot::BeginItem(name.c_str())) {
      if (ImPlot::FitThisFrame()) {
        for (int i = 0; i < candles.size(); ++i) {
            ImPlot::FitPoint(ImPlotPoint(Utils::UTCToUnix(candles[i].begin), candles[i].low - 1));
            ImPlot::FitPoint(ImPlotPoint(Utils::UTCToUnix(candles[i].begin), candles[i].high + 1));
        }
      }
      for (int i = 0; i < candles.size(); i++) {

        double unix_time = static_cast<double>(Utils::UTCToUnix(candles[i].begin));
        ImU32 color = (candles[i].open < candles[i].close) ? red : green;
        ImVec2 open_pos = ImPlot::PlotToPixels(unix_time - half_width, candles[i].open);
        ImVec2 close_pos = ImPlot::PlotToPixels(unix_time + half_width, candles[i].close);

        draw_list->AddRectFilled(open_pos, close_pos, color);

        ImVec2 low_pos = ImPlot::PlotToPixels(unix_time, candles[i].low);
        ImVec2 high_pos = ImPlot::PlotToPixels(unix_time, candles[i].high);
        draw_list->AddLine(low_pos, high_pos, color, 0.25f); 
      }
      
      ImPlot::EndItem();
    }
    
    ImPlot::EndPlot();
  }

  ImGui::End();
}
