#pragma once

#include <iostream>
#include <thread>
#include <mutex>

#include "HttpsClient.hpp"
#include "HttpsTask.hpp"
#include "JsonBuilder.hpp"
#include "BookSnapshot.hpp"
#include "BookUpdate.hpp"
#include "SafeQueue.hpp"
#include "SubscriptionManager.hpp"
#include "SubscriptionRequest.hpp"

class OrderBookManager : public std::enable_shared_from_this<OrderBookManager> {
  public:
    OrderBookManager(SubscriptionManager&, HttpsClient&, std::string&, int64_t);
    ~OrderBookManager();
    BookSnapshot getBookSnapshot();
    void shutdown();
    void init();
  private:
    void processLoop();
    void syncBook();
    bool applyUpdate(std::shared_ptr<BookUpdate>);    

    std::string symbol;
    int64_t id;
        
    std::string event_channel_update = "BOOK_UPDATE:";
    std::string event_channel_snapshot = "BOOK_SNAPSHOT:";
    
    const std::string channel = "depth";
    const std::string speed = "100ms";
    const std::string limit = "20";
    const unsigned int depth = 20;    

    BookSnapshot book;

    SafeQueue<BookUpdate> updates;
    SafeQueue<BookSnapshot> snapshots;
    
    SubscriptionManager& sm;
    SubscriptionRequest update_request;
    SubscriptionRequest snapshot_request;

    HttpsClient& hc;

    mutable std::mutex m;
    std::thread process_thread;
 };

