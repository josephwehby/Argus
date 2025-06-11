#pragma once

#include <iostream>
#include <thread>
#include <mutex>

#include "BookUpdate.hpp"
#include "SafeQueue.hpp"
#include "HttpsClient.hpp"
#include "HttpsTaskType.hpp"
#include "WebSocket.hpp"
#include "JsonBuilder.hpp"
#include "BookSnapshot.hpp"
#include "EventBus.hpp"

class OrderBookManager {
  public:
    OrderBookManager(std::string&, int64_t, std::shared_ptr<WebSocket>, std::shared_ptr<EventBus>, std::shared_ptr<HttpsClient>);
    ~OrderBookManager();
    BookSnapshot getBookSnapshot();
  private:
    void processLoop();
    void syncBook();
    bool applyUpdate(std::shared_ptr<BookUpdate>);    

    std::string symbol;
    int64_t id;
    
    std::string event_channel_update = "BOOK_UPDATE:";
    std::string event_channel_snapshot = "BOOK_SNAPSHOT";

    const std::string speed = "100ms";
    const std::string limit = "20";
    
    BookSnapshot book;

    SafeQueue<BookUpdate> updates;
    SafeQueue<BookSnapshot> snapshots;

    std::shared_ptr<WebSocket> ws;
    std::shared_ptr<EventBus> eb;
    std::shared_ptr<HttpsClient> hc;

    mutable std::mutex m;
    std::thread process_thread;
 };

