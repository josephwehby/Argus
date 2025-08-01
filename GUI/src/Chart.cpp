#include "Chart.hpp"

Chart::Chart(SubscriptionManager& sm_, HttpsClient& hc_, std::string token) : sm(sm_), hc(hc_) {
  symbol = token;
  window_name = "Chart: " + symbol + " ##" + std::to_string(getWindowID()); 
  event_channel = "CANDLE:" + symbol;
  historical_candles_event_channel = "HISTORICAL_CANDLES:" + symbol;
}

Chart::~Chart() {
  sm.unsubscribe(request);
  sm.unsubscribe(historical_request);
}

void Chart::init() {
  std::weak_ptr<Chart> self = shared_from_this();

  HttpsTask task{HttpsTaskType::HistoricalChart, symbol, time_frame, "60"};
  hc.pushRequest(task);

  request = SubscriptionRequest {
    symbol, 
    channel,
    event_channel, 
    window_id,
    [self](const std::shared_ptr<IEvent> update) {
      if (auto locked = self.lock()) {
        auto candle_event = std::dynamic_pointer_cast<Candle>(update);
        if (locked->candles.contains(candle_event->unix_time) && 
            locked->candles[candle_event->unix_time].event_time > candle_event->event_time) return;
        locked->candles[candle_event->unix_time] = *candle_event;
      }
    },
    time_frame,
    false
  };

  sm.subscribe(request);
  
  historical_request = SubscriptionRequest {
    symbol, 
    channel,
    historical_candles_event_channel, 
    window_id,
    [self] (std::shared_ptr<IEvent> update){

      if (auto locked = self.lock()) {
        auto historical_candles_event = std::dynamic_pointer_cast<HistoricalCandles>(update);
        for (const auto& candle : historical_candles_event->candles) {
          locked->candles[candle.unix_time] = candle;
        }
        locked->loading_data = false;
      }
    },
    time_frame,
    true
  };
  
  sm.subscribe(historical_request);
}

void Chart::draw() {

  ImGui::SetNextWindowSize(ImVec2(800, 690), ImGuiCond_FirstUseEver);
  ImGui::Begin(window_name.c_str(), &show, ImGuiWindowFlags_MenuBar);
  
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("View")) {
      if (ImGui::MenuItem("Candlesticks")) show_candles = true;
      if (ImGui::MenuItem("Line")) show_candles = false;
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Time Frame")) {
      if (ImGui::MenuItem("1m")) changeTimeFrame("1m"); 
      if (ImGui::MenuItem("5m")) changeTimeFrame("5m");
      if (ImGui::MenuItem("15m")) changeTimeFrame("15m");
      if (ImGui::MenuItem("1h")) changeTimeFrame("1h"); 
      if (ImGui::MenuItem("1d")) changeTimeFrame("1d");
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Drawing")) {
      if (ImGui::MenuItem("Line")) lines.push_back(std::prev(candles.end())->second.close);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
  
  if (candles.size() > 0) {
    initDraw();
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


void Chart::initDraw() {
  
  double start_time = candles.begin()->first;
  double end_time = std::prev(candles.end())->first;
  double width = .25;
  double half_width = width;
  
  half_width = (candles.size() > 1) ? ((candles.begin()->first - std::next(candles.begin(), 1)->first) * width) : width;

  setStyle();

  if (ImPlot::BeginSubplots("##Stocks", 2, 1, ImVec2(-1,-1),ImPlotSubplotFlags_LinkCols | ImPlotSubplotFlags_NoTitle, ratios)) {

      if (ImPlot::BeginPlot("##Graph", ImVec2(-1,-1), ImPlotFlags_NoFrame | ImPlotFlags_Crosshairs)) {

      ImDrawList* draw_list = ImPlot::GetPlotDrawList();
      
      ImPlot::SetupAxes(0,0,ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks, ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit|ImPlotAxisFlags_Opposite);
      
      ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time);
      ImPlot::SetupAxisLimits(ImAxis_X1, start_time, end_time); 
      
      ImPlot::SetupAxisFormat(ImAxis_Y1, "$%.2f");
      
      ImPlotRect limits = ImPlot::GetPlotLimits();
      long long threshold = Utils::getCandleDurationInSeconds(time_frame) * candles_before_load;

      if (!loading_data && limits.X.Min < (long long)start_time + threshold) {
        HttpsTask task{HttpsTaskType::HistoricalChart, symbol, time_frame, "60", std::to_string(((long long)start_time - (threshold * 12))*1000), std::to_string(1000*(long long)start_time)};
        hc.pushRequest(task);
        loading_data = true;
      }

      if (ImPlot::BeginItem("##Price Chart")) {
        if (show_candles) {
          drawCandles(draw_list, half_width);
        } else {
          drawLine(draw_list);
        }

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
        
        for (int i = 0; i < lines.size(); i++) {
          ImPlot::DragLineY(i, &lines[i], Colors::Yellow_V4, 1);
          ImPlot::TagY(lines[i], Colors::Yellow_V4, true);
        }

        ImPlot::EndItem();
      }
      
      ImPlot::EndPlot();
    }

    drawVolume(half_width); 
    ImPlot::EndSubplots();
  }

}


void Chart::drawLine(ImDrawList* draw_list) {
  
  std::vector<double> price_y(candles.size(), 0);
  std::vector<double> volume_y(candles.size(), 0);
  std::vector<double> time_x(candles.size(), 0);

  int index = 0;
  for (const auto&[time, candle] : candles) {
    price_y[index] = candle.close;
    volume_y[index] = candle.volume;
    time_x[index] = time;
    index++; 
  }
  
  ImPlot::SetNextLineStyle(Colors::Blue_V4, 2);
  ImPlot::PlotLine("##Line Chart", time_x.data(), price_y.data(), price_y.size());
}

void Chart::drawCandles(ImDrawList* draw_list, double half_width) {

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

void Chart::changeTimeFrame(std::string time) {
  if (time == time_frame) return;
  
  candles.clear();
  
  sm.unsubscribe(request);
  sm.unsubscribe(historical_request);

  HttpsTask task{HttpsTaskType::HistoricalChart, symbol, time, "60"};
  hc.pushRequest(task);
  
  loading_data = true; 

  request.params = time;
  historical_request.params = time;

  sm.subscribe(request);
  sm.subscribe(historical_request);

  time_frame = time;
}

