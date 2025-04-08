#include "DataStore.hpp"

DataStore::DataStore() { }

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
