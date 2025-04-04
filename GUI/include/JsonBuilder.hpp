#pragma once

#include <nlohmann/json>

using json = nlohmann::json;

namespace JsonBuilder {

  json generateSubscribe(const std::string& symbol, const std::string& channel, int opt = -1) {
    
    json msg = {
      {"method", "subscribe"},
      {"params", {
        {"channel", "ticker"},
        {"symbol", {symbol}}
      }}
    };

    if (opt != -1) {
      if (channel == "ohlc") msg["params"]["interval"] = opt;
      if (channel == "book") msg["params"]["depth"] = opt;
    }
    
    return msg;
  }

  json generateUnsubscribe(const std::string& symbol, const std::string& channel, int interval = -1) {
    
    json msg = {
      {"method", "unsubscribe"},
      {"params", {
        {"channel", channel},
        {"symbol", {symbol}},
      }}
    };

    if (interval != -1) {
      msg["params"]["interval"] = interval;
    }

    return msg;
  }

};
