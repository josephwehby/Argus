#pragma once

#include "OrderBookManager.hpp"
#include "WebSocket.hpp"
#include "BookUpdate.hpp"

class MasterOrderBookManager {
  public:
    MasterOrderBookManager();
    BookUpdate getOrderBook(const std::string&);
    bool addOrderBook(const std::string&);
    void applyUpdate(BookUpdate&);
  private:
    std::shared_ptr<WebSocket> ws;
    unordered_map<std::string, OrderBookManager> orderbooks;
};
