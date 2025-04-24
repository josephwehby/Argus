#include "Chart.hpp"

Chart::Chart(DataStore& ds, std::shared_ptr<WebSocket> _ws, std::string token) : datastore(ds), ws(_ws) {
  symbol = token;
  window_name = "Chart: " + symbol + " ##" + std::to_string(getWindowID()); 
  
  json sub_msg = JsonBuilder::generateSubscribe(symbol, channel, 1);
  ws->subscribe(sub_msg);
}

Chart::~Chart() {
  json unsub_msg = JsonBuilder::generateUnsubscribe(symbol, channel, 1);
  ws->unsubscribe(unsub_msg);
}

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
