#include "MasterOrderBookManager.hpp"

MasterOrderBookManager::MasterOrderBookManager(std::shared_ptr<WebSocket> ws_) : {}

BookUpdate MasterOrderBookManager::getOrderBook(const std::string& symbol) {

}

bool MasterOrderBookManager::addOrderBook(const std::string& symbol) {
  if (orderbooks.contains(symbol)) {
    std::cerr << "Orderbook for " << symbol << " already exists" << std::endl;
    return false;
  }
  
  orderbooks.emplace(symbol, OrderBookManager(symbol, ws, hc));
}

void MasterOrderBookManager::applyUpdate(BookUpdate& update) {
  if (!orderbooks.contains(update.symbol)) {
    std::cerr << "no orderbook for " << symbole << " exists" << std::endl; 
  }
  orderbooks[update.symbol].pushData(update);
}
