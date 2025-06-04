#pragma once

enum class HttpsTaskType { HistoricalChart, OrderBook, Empty };

struct HttpsTask {
  HttpsTaskType type;
  std::string symbol;
  std::string interval;
  std::string limit;
};
