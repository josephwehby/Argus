// may make this async later

#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "httplib.h"
#include "DataParser.hpp"

using json = nlohmann::json;

class HttpsClient {
  public:
    HttpsClient(std::shared_ptr<DataParser>);
    void getHistoricalChart(const std::string&, const std::string&, const std::string&);
    void getOrderBook(const std::string&, const std::string&);
  private:
    const std::string url = "data-api.binance.vision";
    const std::string endpoint = "/api/v3/";
    
    httplib::SSLClient cli;
    std::shared_ptr<DataParser> dp;
};
