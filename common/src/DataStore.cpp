#include "DataStore.hpp"

DataStore::DataStore() { }

DataStore::~DataStore() {}

/* Ticker access methods */
void DataStore::setTicker(const std::string& symbol, const Level1& ld) {
  std::lock_guard<std::mutex> lock(m);
  ticker_data[symbol] = std::move(ld);
}

Level1 DataStore::getTicker(const std::string& symbol) const {
  std::lock_guard<std::mutex> lock(m);
  if (!ticker_data.contains(symbol)) {
    return Level1{};
  }

  return ticker_data.at(symbol);
}

/* Trade access methods */
void DataStore::setTrade(const std::string& symbol, const Trade& trade) {
  std::lock_guard<std::mutex> lock(m);
  trade_data[symbol].push_back(std::move(trade));
}

std::vector<Trade> DataStore::getTrades(const std::string& symbol) {
  std::lock_guard<std::mutex> lock(m);

  if (!trade_data.contains(symbol)) return {};
  
  auto trades = std::move(trade_data[symbol]);   
  trade_data[symbol].clear();
  
  return trades;
}

/* Book access methods */
void DataStore::setBook(const std::string& symbol, const BookUpdate& book_update) {
  std::lock_guard<std::mutex> lock(m);
  book_data[symbol].bids.insert(book_data[symbol].bids.end(), book_update.bids.begin(), book_update.bids.end());
  book_data[symbol].asks.insert(book_data[symbol].asks.end(), book_update.asks.begin(), book_update.asks.end());
}

BookUpdate DataStore::getBook(const std::string& symbol) {
  std::lock_guard<std::mutex> lock(m);
  if (!book_data.contains(symbol)) return BookUpdate{};

  auto book = std::move(book_data[symbol]);
  book_data.erase(symbol);

  return book;
}

/* Candle access methods */
void DataStore::setCandle(const std::string& symbol, const Candle& candle) {
  std::lock_guard<std::mutex> lock(m);
  candle_data[symbol].push_back(std::move(candle));
}

std::vector<Candle> DataStore::getCandles(const std::string& symbol) {
  std::lock_guard<std::mutex> lock(m);
  if (!candle_data.contains(symbol)) return {};
  
  auto candles = std::move(candle_data[symbol]);   
  candle_data[symbol].clear();

  return candles;

}
