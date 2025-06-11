#pragma once

#include <mutex>
#include <unordered_map>
#include <iostream>

#include "OrderBookManager.hpp"
#include "WebSocket.hpp"
#include "DataParser.hpp"
#include "BookUpdate.hpp"
#include "BookSnapshot.hpp"

class MasterOrderBookManager {
  public:
    MasterOrderBookManager(std::shared_ptr<WebSocket>, std::shared_ptr<DataParser>);
    BookSnapshot getOrderBook(const std::string&);
    bool addOrderBook(const std::string&);
    void applyUpdate(BookUpdate&);
    void applySnapshot(BookSnapshot&); 
  private:
    std::shared_ptr<WebSocket> ws;
    std::shared_ptr<DataParser> dp;
    std::unordered_map<std::string, OrderBookManager> orderbooks;
    mutable std::mutex m;
};
