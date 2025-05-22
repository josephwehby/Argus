// may make this async later

#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "httplib.h"

using json = nlohmann::json;

class HttpsClient {
  public:
    HttpsClient();
    void getHistoricalChart(const std::string&, const std::string&, const std::string&);
  private:
    const std::string url = "https://data-api.binance.vision";
    const std::string enpoint = "/api/v3/";

    httplib::SSLClient cli;
};
