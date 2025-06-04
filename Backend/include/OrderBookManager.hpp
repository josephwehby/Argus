#pragma once

#include <iostream>
#include <thread>
#include <mutex>

#include "BookUpdate.hpp"
#include "SafeQueue.hpp"
#include "HttpsClient.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"
#include "BookSnapshot.hpp"

class OrderBookManager {
  public:
    OrderBookManager(std::string&, std::shared_ptr<WebSocket>, std::shared_ptr<DataParser>);
    ~OrderBookManager();
    void pushUpdate(BookUpdate&);
    void pushSnapshot(BookSnapshot&);
    BookSnapshot getBookSnapshot();
  private:
    void processLoop();
    void syncBook();
    bool applyUpdate(std::shared_ptr<BookUpdate>);    

    std::string symbol;
    const std::string speed = "100ms";
    const std::string limit = "20";
    
    BookSnapshot book;

    SafeQueue<BookUpdate> updates;
    SafeQueue<BookSnapshot> snapshots;

    HttpsClient hc;
    std::shared_ptr<WebSocket> ws;
    std::shared_ptr<DataParser> dp;

    mutable std::mutex m;
    std::thread process_thread;
 };

