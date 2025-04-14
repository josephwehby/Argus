#include "DataStore.hpp"

DataStore::DataStore() { }

/* Ticker access methods */
void DataStore::setTicker(const std::string& symbol, std::shared_ptr<Level1> ld) {
  std::lock_guard<std::mutex> lock(m);
  ticker_data[symbol] = ld;
}

std::shared_ptr<Level1> DataStore::getTicker(const std::string& symbol) const {
  std::lock_guard<std::mutex> lock(m);
  if (!ticker_data.contains(symbol)) {
    return nullptr;
  }

  return ticker_data.at(symbol);
}

/* Trade access methods */
void DataStore::setTrades(const std::string& symbol, const std::vector<Trade>& trades) {
  std::lock_guard<std::mutex> lock(m);
  trade_data[symbol].insert(trade_data[symbol].end(), trades.begin(), trades.end());
}

std::vector<Trade> DataStore::getTrades(const std::string& symbol) const {
  std::lock_guard<std::mutex> lock(m);

  if (!trade_data.contains(symbol)) return {};
  
  return trade_data.at(symbol);
}
