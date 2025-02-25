#include "Chart.hpp"
#include <ctime>

Chart::Chart(std::string token) {
  symbol = token;
  window_name = "Chart: " + symbol + " ##" + std::to_string(getWindowID()); 
  
  candles = {
    {97.5, 99.2, 97.1, 98.7, 1200.5, 1, "2025-02-19T09:00:00"},
    {98.7, 99.5, 98.2, 99.1, 1500.8, 1, "2025-02-19T09:01:00"}, 
    {99.1, 100.0, 98.9, 99.6, 1800.3, 1, "2025-02-19T09:02:00"},
    {99.6, 99.9, 98.8, 99.0, 1600.7, 1, "2025-02-19T09:03:00"},
    {99.0, 99.3, 97.9, 98.5, 1400.2, 1, "2025-02-19T09:04:00"},
    {98.5, 98.9, 97.7, 98.0, 1300.6, 1, "2025-02-19T09:05:00"},
    {98.0, 98.6, 97.5, 97.9, 1100.9, 1, "2025-02-19T09:06:00"},
    {97.9, 98.4, 97.6, 98.2, 1050.4, 1, "2025-02-19T09:07:00"},
    {98.2, 99.0, 98.0, 98.8, 1700.1, 1, "2025-02-19T09:08.00"},
    {98.8, 99.7, 98.4, 99.5, 1900.5, 1, "2025-02-19T09:09:00"}
  };

}

Chart::~Chart() {}

void Chart::draw() {
  ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
  ImGui::Begin(window_name.c_str(), &show);
  const double half_width = 10;

  if(ImPlot::BeginPlot("##Graph", ImVec2(-1,-1), ImPlotFlags_NoLegend | ImPlotFlags_NoFrame)) {

    ImDrawList* draw_list = ImPlot::GetPlotDrawList();
    ImPlot::SetupAxes(0,0,ImPlotAxisFlags_NoGridLines, ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit|ImPlotAxisFlags_Opposite);
    ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time);
    ImPlot::SetupAxisLimits(ImAxis_X1, Utils::UTCToUnix(candles[0].begin), Utils::UTCToUnix(candles.back().begin) + 5);
    ImPlot::SetupAxisFormat(ImAxis_Y1, "$%.0f");

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
