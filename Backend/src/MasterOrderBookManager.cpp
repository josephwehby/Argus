#include "MasterOrderBookManager.hpp"

MasterOrderBookManager::MasterOrderBookManager(std::shared_ptr<WebSocket> ws_, std::shared_ptr<DataParser> dp_) : ws(ws_), dp(dp_) {}

BookSnapshot MasterOrderBookManager::getOrderBook(const std::string& symbol) {
  std::lock_guard<std::mutex> lock(m);
  if (!orderbooks.contains(symbol)) {
    return {};
  }

  BookSnapshot snapshot = orderbooks[symbol].getBookSnapshot();
  return snapshot;
}

bool MasterOrderBookManager::addOrderBook(const std::string& symbol) {
  std::lock_guard<std::mutex> lock(m);
  if (orderbooks.contains(symbol)) {
    std::cerr << "Orderbook for " << symbol << " already exists" << std::endl;
    return false;
  }
  
  orderbooks.emplace(symbol, OrderBookManager(symbol, ws, dp));
}

void MasterOrderBookManager::applyUpdate(BookUpdate& update) {
  std::lock_guard<std::mutex> lock(m);
  if (!orderbooks.contains(update.symbol)) {
    std::cerr << "no orderbook for " << symbole << " exists" << std::endl; 
  }
  orderbooks[update.symbol].pushUpdate(update);
}

void MasterOrderBookManager::applySnapshot(BookSnapshot& snapshot) {
  std::lock_guard<std::mutex> lock(m);
  if (!orderbooks.contains(snapshot.symbol)) {
    std::cerr << "no orderbook for " << symbole << " exists" << std::endl; 
  }
  orderbooks[update.symbol].pushSnapshot(update);
}
