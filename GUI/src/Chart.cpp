#include "Chart.hpp"

Chart::Chart(DataStore& ds, std::string token) : datastore(ds) {
  symbol = token;
  window_name = "Chart: " + symbol + " ##" + std::to_string(getWindowID()); 
  
  candles = {
    {97500.0, 99200.0, 97100.0, 98700.0, 120050.0, 1, "2025-02-19T09:00:00"},
    {98700.0, 99500.0, 98200.0, 99100.0, 150080.0, 1, "2025-02-19T09:01:00"},
    {99100.0, 100000.0, 98900.0, 99600.0, 180030.0, 1, "2025-02-19T09:02:00"},
    {99600.0, 99900.0, 98800.0, 99000.0, 160070.0, 1, "2025-02-19T09:03:00"},
    {99000.0, 99300.0, 97900.0, 98500.0, 140020.0, 1, "2025-02-19T09:04:00"},
    {98500.0, 98900.0, 97700.0, 98000.0, 130060.0, 1, "2025-02-19T09:05:00"},
    {98000.0, 98600.0, 97500.0, 97900.0, 110090.0, 1, "2025-02-19T09:06:00"},
    {97900.0, 98400.0, 97600.0, 98200.0, 105040.0, 1, "2025-02-19T09:07:00"},
    {98200.0, 99000.0, 98000.0, 98800.0, 170010.0, 1, "2025-02-19T09:08:00"},
    {98800.0, 99700.0, 98400.0, 99500.0, 190050.0, 1, "2025-02-19T09:09:00"},
    {99500.0, 100200.0, 99100.0, 100000.0, 210080.0, 1, "2025-02-19T09:10:00"},
    {100000.0, 100800.0, 99700.0, 100300.0, 220040.0, 1, "2025-02-19T09:11:00"},
    {100300.0, 101000.0, 99900.0, 100700.0, 240010.0, 1, "2025-02-19T09:12:00"},
    {100700.0, 101500.0, 100200.0, 101100.0, 260030.0, 1, "2025-02-19T09:13:00"},
    {101100.0, 101800.0, 100900.0, 101400.0, 250070.0, 1, "2025-02-19T09:14:00"},
    {101400.0, 101900.0, 100800.0, 101000.0, 230050.0, 1, "2025-02-19T09:15:00"},
    {101000.0, 101400.0, 100300.0, 100600.0, 210090.0, 1, "2025-02-19T09:16:00"},
    {100600.0, 101200.0, 100000.0, 100800.0, 200040.0, 1, "2025-02-19T09:17:00"},
    {100800.0, 101600.0, 100500.0, 101300.0, 270020.0, 1, "2025-02-19T09:18:00"},
    {101300.0, 102000.0, 100900.0, 101700.0, 290010.0, 1, "2025-02-19T09:19:00"},
    {101700.0, 102400.0, 101200.0, 101900.0, 300060.0, 1, "2025-02-19T09:20:00"},
    {101900.0, 102700.0, 101500.0, 102300.0, 320080.0, 1, "2025-02-19T09:21:00"},
    {102300.0, 103000.0, 101800.0, 102600.0, 340040.0, 1, "2025-02-19T09:22:00"},
    {102600.0, 103500.0, 102000.0, 103100.0, 360020.0, 1, "2025-02-19T09:23:00"},
    {103100.0, 103800.0, 102700.0, 103500.0, 380030.0, 1, "2025-02-19T09:24:00"},
    {103500.0, 104200.0, 103000.0, 103800.0, 400050.0, 1, "2025-02-19T09:25:00"},
    {103800.0, 104500.0, 103300.0, 104100.0, 420090.0, 1, "2025-02-19T09:26:00"},
    {104100.0, 105000.0, 103700.0, 104600.0, 450010.0, 1, "2025-02-19T09:27:00"},
    {104600.0, 105300.0, 104000.0, 104900.0, 470020.0, 1, "2025-02-19T09:28:00"},
    {104900.0, 105700.0, 104500.0, 105200.0, 500070.0, 1, "2025-02-19T09:29:00"}
  };

}

Chart::~Chart() {}

// we should generate the unix time when reading the data in rather than making all these extra utctounix calls each frame
void Chart::draw() {
  ImGui::SetNextWindowSize(ImVec2(800, 690), ImGuiCond_Always);
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
