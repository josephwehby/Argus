#include "Chart.hpp"

int TimeFormatter(double value, char* buff, int size, void*) {
  std::time_t t = static_cast<std::time_t>(value);
  std::tm local_tm;
  localtime_s(&local_tm, &t);

  return std::strftime(buff, size, "%I:%M", &local_tm);
}

Chart::Chart(std::shared_ptr<DataStore> ds, std::shared_ptr<WebSocket> _ws, std::string token) : datastore(ds), ws(_ws) {
  symbol = token;
  window_name = "Chart: " + symbol + " ##" + std::to_string(getWindowID()); 
  
  json sub_msg = JsonBuilder::generateSubscribe(symbol, channel, 1);
  ws->subscribe(sub_msg);
}

Chart::~Chart() {
  json unsub_msg = JsonBuilder::generateUnsubscribe(symbol, channel, 1);
  ws->unsubscribe(unsub_msg);
}

void Chart::updateCandles() {
  auto updates = datastore->getCandles(symbol);
  
  for (auto update : updates) {
    candles[update.unix_time] = update;
  }
}

void Chart::draw() {

  updateCandles(); 
  
  if (candles.size() == 0) return;

  ImGui::SetNextWindowSize(ImVec2(800, 690), ImGuiCond_FirstUseEver);
  ImGui::Begin(window_name.c_str(), &show);

  double width = .25;
  double half_width = width;
  
  std::vector<double> volume_x(candles.size());
  std::vector<double> volume_y(candles.size());

  if (ImPlot::BeginSubplots("##Stocks", 2, 1, ImVec2(-1,-1),ImPlotSubplotFlags_LinkCols | ImPlotSubplotFlags_NoTitle, ratios)) {
    
    half_width = (candles.size() > 1) ? ((candles.begin()->first - std::next(candles.begin(), 1)->first) * width) : width;

    ImPlotStyle& style = ImPlot::GetStyle();
    style.Colors[ImPlotCol_PlotBg] = background_color;
    style.Colors[ImPlotCol_Crosshairs] = ImVec4(1,1,1,0.5f);

    style.PlotPadding = ImVec2(5, 5);
    style.LabelPadding = ImVec2(10, 10);
    style.LegendPadding = ImVec2(0, 0);
    style.LegendInnerPadding = ImVec2(0, 0);
    style.MousePosPadding = ImVec2(0, 0);
    style.AnnotationPadding = ImVec2(0, 0);
    style.FitPadding = ImVec2(0, 0);

    if (ImPlot::BeginPlot("##Graph", ImVec2(-1,-1), ImPlotFlags_NoFrame | ImPlotFlags_Crosshairs)) {

      ImDrawList* draw_list = ImPlot::GetPlotDrawList();
      ImPlot::SetupAxes(0,0,ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_Opposite);

      ImPlot::SetupAxisLimits(ImAxis_X1, candles.begin()->first, std::prev(candles.end())->first);
      ImPlot::SetupAxisFormat(ImAxis_Y1, "$%.2f");
      ImPlot::SetupAxisFormat(ImAxis_X1, TimeFormatter);

      if (ImPlot::BeginItem("##Price Chart")) {
        if (ImPlot::FitThisFrame()) {

          for (const auto& [time, candle] : candles) {
              ImPlot::FitPoint(ImPlotPoint(time, candle.low - 10));
              ImPlot::FitPoint(ImPlotPoint(time, candle.high + 10));
          }
        }
        
        int index = 0;
        for (const auto& [time, candle] : candles) {
          
          // this is to align it to the left. implot auto centers it
          volume_x[index] = time+half_width;
          volume_y[index] = candle.volume;
          
          ImU32 color = (candle.open <= candle.close) ? green : red;
          ImVec2 open_pos = ImPlot::PlotToPixels(time, candle.open);
          ImVec2 close_pos = ImPlot::PlotToPixels(time + (2*half_width), candle.close);

          draw_list->AddRectFilled(open_pos, close_pos, color);

          ImVec2 low_pos = ImPlot::PlotToPixels(time+half_width, candle.low);
          ImVec2 high_pos = ImPlot::PlotToPixels(time+half_width, candle.high);
          draw_list->AddLine(low_pos, high_pos, color, half_width/2); 
          index++;
        }
        

        // from close to edge of graph?
        auto it = std::prev(candles.end());
        ImU32 color = (it->second.open <= it->second.close) ? green : red;

        ImPlotRect limits = ImPlot::GetPlotLimits();

        std::string current_price = Utils::formatPrice(it->second.close);
        ImVec2 current_price_size = ImGui::CalcTextSize(current_price.c_str());
        
        ImVec2 price_pos = ImPlot::PlotToPixels(limits.Max().x, it->second.close);
        price_pos.x -= (current_price_size.x + 20);
        price_pos.y -= current_price_size.y;
        draw_list->AddText(price_pos, color, current_price.c_str()); 
        
        ImVec2 max = ImPlot::PlotToPixels(limits.Max().x, it->second.close); 
        ImVec2 min = ImPlot::PlotToPixels(limits.Min().x, it->second.close);
        draw_list->AddLine(min, max, color, 1);

        ImPlot::EndItem();
      }
      
      ImPlot::EndPlot();
    }

    if (ImPlot::BeginPlot("##Volume Plot", ImVec2(-1,-1), ImPlotFlags_Crosshairs)) {
      ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_RangeFit | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels);
      ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoGridLines |  ImPlotAxisFlags_RangeFit | ImPlotAxisFlags_Opposite);

      ImPlot::SetupAxisFormat(ImAxis_X1, TimeFormatter);
      ImPlot::SetupAxisLimits(ImAxis_X1, candles.begin()->first, std::prev(candles.end())->first + 5);

      ImPlot::SetNextFillStyle(ImVec4(0.7f, 0.6f, 0.8f, 0.7f));
      ImPlot::PlotBars("##", volume_x.data(), volume_y.data(), volume_x.size(), half_width*2);
      ImPlot::EndPlot();
    }

    ImPlot::EndSubplots();
  }
  ImGui::End();
}
