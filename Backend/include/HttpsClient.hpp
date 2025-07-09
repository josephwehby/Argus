#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <memory>
#include <thread>

#include "httplib.h"
#include "DataParser.hpp"
#include "SafeQueue.hpp"
#include "HttpsTask.hpp"

using json = nlohmann::json;

class HttpsClient {
  public:
    HttpsClient(DataParser&);
    ~HttpsClient();
    void pushRequest(HttpsTask&);
    void shutdown();
  private:
    void processLoop();
    void getHistoricalChart(std::shared_ptr<HttpsTask>);
    void getOrderBook(std::shared_ptr<HttpsTask>);
     
    const std::string url = "data-api.binance.vision";
    const std::string endpoint = "/api/v3/";
    
    httplib::SSLClient cli;
    std::thread process_thread;
    DataParser& dp;
    SafeQueue<HttpsTask> events;
};
