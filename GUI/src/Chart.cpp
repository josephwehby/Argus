#include "Chart.hpp"

Chart::Chart(std::shared_ptr<DataStore> _ds, std::shared_ptr<WebSocket> _ws, std::shared_ptr<HttpsClient> _hc, std::string token) : ds(_ds), ws(_ws), hc(_hc) {

  symbol = token;
  window_name = "Chart: " + symbol + " ##" + std::to_string(getWindowID()); 
  
  HttpsTask task{HttpsTaskType::HistoricalChart, symbol, "1m", "10"};
  hc->pushRequest(task);

  json sub_msg = JsonBuilder::generateSubscribe(symbol, channel, window_id, "1m");
  ws->subscribe(sub_msg);
}

Chart::~Chart() {
  json unsub_msg = JsonBuilder::generateUnsubscribe(symbol, channel, window_id, "1m");
  ws->unsubscribe(unsub_msg);
}

void Chart::updateCandles() {
  auto updates = ds->getCandles(symbol);
  
  for (auto update : updates) {
    candles[update.unix_time] = update;
  }
}

void Chart::draw() {

  updateCandles(); 
  
  ImGui::SetNextWindowSize(ImVec2(800, 690), ImGuiCond_FirstUseEver);
  ImGui::Begin(window_name.c_str(), &show);
  
  if (ImGui::Button("Switch View")) {
    show_candles = (show_candles == true) ? false : true;
  }

  if (candles.size() > 0) {
    if (show_candles) {
      drawCandles();
    } else {
      drawLine();
    }
  }

  ImGui::End();
}

void Chart::setStyle() {
  ImPlotStyle& style = ImPlot::GetStyle();
  
  style.Colors[ImPlotCol_PlotBg] = Colors::Background_V4;
  style.Colors[ImPlotCol_Crosshairs] = Crosshair_Color;

  style.PlotPadding = ImVec2(5, 5);
  style.LabelPadding = ImVec2(10, 10);
  style.LegendPadding = ImVec2(0, 0);
  style.LegendInnerPadding = ImVec2(0, 0);
  style.MousePosPadding = ImVec2(0, 0);
  style.AnnotationPadding = ImVec2(0, 0);
  style.FitPadding = ImVec2(0, 0);
  style.UseLocalTime = true;
}

void Chart::drawLine() {
  std::vector<double> price_y(candles.size(), 0);
  std::vector<double> volume_y(candles.size(), 0);
  std::vector<double> time_x(candles.size(), 0);

  double width = .25;
  double half_width = width;
  half_width = (candles.size() > 1) ? ((candles.begin()->first - std::next(candles.begin(), 1)->first) * width) : width;

  int index = 0;
  for (const auto&[time, candle] : candles) {
    price_y[index] = candle.close;
    volume_y[index] = candle.volume;
    time_x[index] = time;
    index++; 
  }
  
  setStyle();

  if (ImPlot::BeginSubplots("##Stocks", 2, 1, ImVec2(-1,-1),ImPlotSubplotFlags_LinkCols | ImPlotSubplotFlags_NoTitle, ratios)) {

        if (ImPlot::BeginPlot("##Graph", ImVec2(-1,-1), ImPlotFlags_NoFrame | ImPlotFlags_Crosshairs)) {

      ImDrawList* draw_list = ImPlot::GetPlotDrawList();
      
      ImPlot::SetupAxes(0,0,ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks, ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit|ImPlotAxisFlags_Opposite);
      
      ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time);
      ImPlot::SetupAxisLimits(ImAxis_X1, time_x[0], time_x.back());
      
      ImPlot::SetupAxisFormat(ImAxis_Y1, "$%.2f");

      if (ImPlot::BeginItem("##Price Chart")) {
        ImPlot::PushStyleColor(ImPlotCol_Fill, Colors::Blue_V4);
        ImPlot::PlotLine("##Line Chart", time_x.data(), price_y.data(), price_y.size());
        ImPlot::PlotShaded("##Shaded Chart", time_x.data(), price_y.data(), price_y.size(), -INFINITY);
        ImPlot::PopStyleColor();

        auto it = std::prev(candles.end());
        
        ImU32 color = (it->second.open <= it->second.close) ? Colors::Green_U32 : Colors::Red_U32;
        ImVec4 tag_color = (it->second.open <= it->second.close) ? Colors::Green_V4 : Colors::Red_V4;

        ImPlotRect limits = ImPlot::GetPlotLimits();

        std::string current_price = Utils::formatPrice(it->second.close);
        ImVec2 current_price_size = ImGui::CalcTextSize(current_price.c_str());

        ImPlot::TagY(it->second.close, tag_color, true);
        ImVec2 max = ImPlot::PlotToPixels(limits.Max().x, it->second.close); 
        ImVec2 min = ImPlot::PlotToPixels(limits.Min().x, it->second.close);
        draw_list->AddLine(min, max, color, .5);

        ImPlot::EndItem();
      }
      
      ImPlot::EndPlot();
    }

    drawVolume(half_width); 

    ImPlot::EndSubplots();
  }

}

void Chart::drawCandles() {

  double width = .25;
  double half_width = width;
  
  if (ImPlot::BeginSubplots("##Stocks", 2, 1, ImVec2(-1,-1),ImPlotSubplotFlags_LinkCols | ImPlotSubplotFlags_NoTitle, ratios)) {
    
    half_width = (candles.size() > 1) ? ((candles.begin()->first - std::next(candles.begin(), 1)->first) * width) : width;

    setStyle();

    if (ImPlot::BeginPlot("##Graph", ImVec2(-1,-1), ImPlotFlags_NoFrame | ImPlotFlags_Crosshairs)) {

      ImDrawList* draw_list = ImPlot::GetPlotDrawList();
      ImPlot::SetupAxes(0,0,ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks, ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit|ImPlotAxisFlags_Opposite);
      
      ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time);
      ImPlot::SetupAxisLimits(ImAxis_X1, candles.begin()->first, std::prev(candles.end())->first);
      
      ImPlot::SetupAxisFormat(ImAxis_Y1, "$%.2f");

      if (ImPlot::BeginItem("##Price Chart")) {

        if (ImPlot::FitThisFrame()) {
          for (const auto& [time, candle] : candles) {
              ImPlot::FitPoint(ImPlotPoint(time, candle.low - 5));
              ImPlot::FitPoint(ImPlotPoint(time, candle.high + 5));
          }
        }

        for (const auto& [time, candle] : candles) {
          ImU32 color = (candle.open <= candle.close) ? Colors::Green_U32 : Colors::Red_U32;

          ImVec2 open_pos = ImPlot::PlotToPixels(time - half_width, candle.open);
          ImVec2 close_pos = ImPlot::PlotToPixels(time + half_width, candle.close);

          draw_list->AddRectFilled(open_pos, close_pos, color);

          ImVec2 low_pos = ImPlot::PlotToPixels(time, candle.low);
          ImVec2 high_pos = ImPlot::PlotToPixels(time, candle.high);
          draw_list->AddLine(low_pos, high_pos, color, half_width/2); 
        }

        // from close to edge of graph?
        auto it = std::prev(candles.end());
        
        ImU32 color = (it->second.open <= it->second.close) ? Colors::Green_U32 : Colors::Red_U32;
        ImVec4 tag_color = (it->second.open <= it->second.close) ? Colors::Green_V4 : Colors::Red_V4;

        ImPlotRect limits = ImPlot::GetPlotLimits();

        std::string current_price = Utils::formatPrice(it->second.close);
        ImVec2 current_price_size = ImGui::CalcTextSize(current_price.c_str());

        ImPlot::TagY(it->second.close, tag_color, true);
        ImVec2 max = ImPlot::PlotToPixels(limits.Max().x, it->second.close); 
        ImVec2 min = ImPlot::PlotToPixels(limits.Min().x, it->second.close);
        draw_list->AddLine(min, max, color, .5);

        ImPlot::EndItem();
      }
      
      ImPlot::EndPlot();
    }
    
    drawVolume(half_width);
    
    ImPlot::EndSubplots();
  }
}

void Chart::drawVolume(double half_width) {
  if (ImPlot::BeginPlot("##Volume Plot", ImVec2(-1,-1), ImPlotFlags_Crosshairs)) {
    ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks);
    ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_Opposite | ImPlotAxisFlags_AutoFit);
      

    ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time);
    ImPlot::SetupAxisLimits(ImAxis_X1, candles.begin()->first, std::prev(candles.end())->first + 5);

    ImDrawList* draw_list = ImPlot::GetPlotDrawList();

    if (ImPlot::BeginItem("## Volume Bars")) {
      
      if (ImPlot::FitThisFrame()) {
        for (const auto& [time, candle] : candles) {
            ImPlot::FitPoint(ImPlotPoint(time, 0));
            ImPlot::FitPoint(ImPlotPoint(time, candle.volume + 5));
        }
      }

      for (const auto& [time, candle] : candles) {
        ImVec2 buy_bottom = ImPlot::PlotToPixels(time - half_width, 0);
        ImVec2 buy_top = ImPlot::PlotToPixels(time + half_width, candle.buy_volume);
        ImVec2 sell_bottom = ImPlot::PlotToPixels(time - half_width, candle.buy_volume);
        ImVec2 sell_top = ImPlot::PlotToPixels(time + half_width, candle.volume);

        draw_list->AddRectFilled(buy_bottom, buy_top, Colors::Green_U32);
        draw_list->AddRectFilled(sell_bottom, sell_top, Colors::Red_U32);
      }
      
    }
    ImPlot::EndPlot();
  }
}
