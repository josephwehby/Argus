#include "Chart.hpp"

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
    {98.2, 99.0, 98.0, 98.8, 1700.1, 1, "2025-02-19T09:08:00"},
    {98.8, 99.7, 98.4, 99.5, 1900.5, 1, "2025-02-19T09:09:00"},
    {99.5, 100.2, 99.1, 100.0, 2100.8, 1, "2025-02-19T09:10:00"},
    {100.0, 100.8, 99.7, 100.3, 2200.4, 1, "2025-02-19T09:11:00"},
    {100.3, 101.0, 99.9, 100.7, 2400.1, 1, "2025-02-19T09:12:00"},
    {100.7, 101.5, 100.2, 101.1, 2600.3, 1, "2025-02-19T09:13:00"},
    {101.1, 101.8, 100.9, 101.4, 2500.7, 1, "2025-02-19T09:14:00"},
    {101.4, 101.9, 100.8, 101.0, 2300.5, 1, "2025-02-19T09:15:00"},
    {101.0, 101.4, 100.3, 100.6, 2100.9, 1, "2025-02-19T09:16:00"},
    {100.6, 101.2, 100.0, 100.8, 2000.4, 1, "2025-02-19T09:17:00"},
    {100.8, 101.6, 100.5, 101.3, 2700.2, 1, "2025-02-19T09:18:00"},
    {101.3, 102.0, 100.9, 101.7, 2900.1, 1, "2025-02-19T09:19:00"},
    {101.7, 102.4, 101.2, 101.9, 3000.6, 1, "2025-02-19T09:20:00"},
    {101.9, 102.7, 101.5, 102.3, 3200.8, 1, "2025-02-19T09:21:00"},
    {102.3, 103.0, 101.8, 102.6, 3400.4, 1, "2025-02-19T09:22:00"},
    {102.6, 103.5, 102.0, 103.1, 3600.2, 1, "2025-02-19T09:23:00"},
    {103.1, 103.8, 102.7, 103.5, 3800.3, 1, "2025-02-19T09:24:00"},
    {103.5, 104.2, 103.0, 103.8, 4000.5, 1, "2025-02-19T09:25:00"},
    {103.8, 104.5, 103.3, 104.1, 4200.9, 1, "2025-02-19T09:26:00"},
    {104.1, 105.0, 103.7, 104.6, 4500.1, 1, "2025-02-19T09:27:00"},
    {104.6, 105.3, 104.0, 104.9, 4700.2, 1, "2025-02-19T09:28:00"},
    {104.9, 105.7, 104.5, 105.2, 5000.7, 1, "2025-02-19T09:29:00"}
  };

}

Chart::~Chart() {}

// we should generate the unix time when reading the data in rather than making all these extra utctounix calls each frame
void Chart::draw() {
  ImGui::SetNextWindowSize(ImVec2(800, 660), ImGuiCond_Always);
  ImGui::Begin(window_name.c_str(), &show);

  double width = .25;
  double half_width = width;

  std::vector<double> volume_x(candles.size());
  std::vector<double> volume_y(candles.size());

  if (ImPlot::BeginSubplots("##Stocks", 2, 1, ImVec2(-1,-1),ImPlotSubplotFlags_LinkCols | ImPlotSubplotFlags_NoTitle, ratios)) {

    half_width = (candles.size() > 1) ? ((Utils::UTCToUnix(candles[0].begin) - Utils::UTCToUnix(candles[1].begin)) * width) : width;

    ImPlotStyle& style = ImPlot::GetStyle();
    style.Colors[ImPlotCol_PlotBg] = ImVec4(0.08f, 0.098f, 0.11f, 1.0f);
    style.Colors[ImPlotCol_Crosshairs] = ImVec4(1,1,1,0.5f);

    style.PlotPadding = ImVec2(5, 5);
    style.LabelPadding = ImVec2(10, 10);
    style.LegendPadding = ImVec2(0, 0);
    style.LegendInnerPadding = ImVec2(0, 0);
    style.MousePosPadding = ImVec2(0, 0);
    style.AnnotationPadding = ImVec2(0, 0);
    style.FitPadding = ImVec2(0, 0);

    if (ImPlot::BeginPlot("##Graph", ImVec2(-1,-1), ImPlotFlags_NoFrame | ImPlotFlags_Crosshairs | ImPlotFlags_NoBoxSelect)) {

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

          ImU32 color = (candles[i].open <= candles[i].close) ? green : red;
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
