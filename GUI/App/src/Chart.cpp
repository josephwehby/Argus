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

// we should generate the unix time when reading the data in rather than making all these extra utctounix calls each frame
void Chart::draw() {
  ImGui::SetNextWindowSize(ImVec2(820, 560), ImGuiCond_Always);
  ImGui::Begin(window_name.c_str(), &show);
  const double half_width = 8;

  std::vector<double> volume_x(candles.size());
  std::vector<double> volume_y(candles.size());

  if (ImPlot::BeginSubplots("##Stocks", 2, 1, ImVec2(-1,-1),ImPlotSubplotFlags_LinkCols | ImPlotSubplotFlags_NoTitle, ratios)) {


    ImPlotStyle& style = ImPlot::GetStyle();
    style.Colors[ImPlotCol_PlotBg] = ImVec4(0.08f, 0.098f, 0.11f, 1.0f);
    style.Colors[ImPlotCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImPlotCol_FrameBg] = ImVec4(0.08f, 0.098f, 0.11f, 1.0f);

    style.PlotPadding = ImVec2(0, 0);
    style.LabelPadding = ImVec2(10, 10);
    style.LegendPadding = ImVec2(0, 0);
    style.LegendInnerPadding = ImVec2(0, 0);
    style.MousePosPadding = ImVec2(0, 0);
    style.AnnotationPadding = ImVec2(0, 0);
    style.FitPadding = ImVec2(0, 0);

    if (ImPlot::BeginPlot("##Graph")) {

      ImDrawList* draw_list = ImPlot::GetPlotDrawList();
      ImPlot::SetupAxes(0,0,ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit|ImPlotAxisFlags_Opposite);
      ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time);

      ImPlot::SetupAxisLimits(ImAxis_X1, Utils::UTCToUnix(candles[0].begin), Utils::UTCToUnix(candles.back().begin));
      ImPlot::SetupAxisFormat(ImAxis_Y1, "$%.2f");

      if (ImPlot::BeginItem("##Price Chart")) {
        if (ImPlot::FitThisFrame()) {
          for (int i = 0; i < candles.size(); ++i) {
              ImPlot::FitPoint(ImPlotPoint(Utils::UTCToUnix(candles[i].begin), candles[i].low - 1));
              ImPlot::FitPoint(ImPlotPoint(Utils::UTCToUnix(candles[i].begin), candles[i].high + 1));
          }
        }

        for (int i = 0; i < candles.size(); i++) {

          double unix_time = static_cast<double>(Utils::UTCToUnix(candles[i].begin));
          
          volume_x[i] = unix_time;
          volume_y[i] = candles[i].volume;

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

    if (ImPlot::BeginPlot("##Volume Plot")) {
      ImPlot::SetupAxes(0,0,ImPlotAxisFlags_NoGridLines,ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit|ImPlotAxisFlags_Opposite);
      ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time);
      ImPlot::SetupAxisLimits(ImAxis_X1, Utils::UTCToUnix(candles[0].begin), Utils::UTCToUnix(candles.back().begin) + 5);
      ImPlot::SetNextFillStyle(ImVec4(1.f,0.75f,0.25f,1));
      ImPlot::PlotBars("##", volume_x.data(), volume_y.data(), volume_x.size(), half_width);
      ImPlot::EndPlot();
    }

    ImPlot::EndSubplots();
  }
    
  ImGui::End();
}
