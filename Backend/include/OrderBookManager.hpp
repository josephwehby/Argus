#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <map>

#include "BookUpdate.hpp"
#include "SafeQueue.hpp"
#include "HttpsClient.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"

class OrderBookManager {
  public:
    OrderBookManager(std::string&, std::shared_ptr<WebSocket>, std::shared_ptr<HttpsClient>);
    ~OrderBookManager();
    void pushData(BookUpdate&);
  private:
    void processLoop();
    void syncBook();
    
    std::string symbol;
    const std::string speed = "100ms";
    const std::string limit = "20";

    SafeQueue<BookUpdate> events;
    std::shared_ptr<WebSocket> ws;
    HttpsClient hc;

    std::map<double, double> bids;
    std::map<double, double> asks;
};

