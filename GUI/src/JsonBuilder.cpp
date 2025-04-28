#include "JsonBuilder.hpp"

namespace JsonBuilder {
  json generateSubscribe(const std::string& symbol, const std::string& channel, int opt) {
      
    json msg = {
      {"method", "subscribe"},
      {"params", {
        {"channel", channel},
        {"symbol", {symbol}}
      }}
    };

    if (opt != -1) {
      if (channel == "ohlc") msg["params"]["interval"] = opt;
      if (channel == "book") msg["params"]["depth"] = opt;
    }
    
    return msg;
  }

  json generateUnsubscribe(const std::string& symbol, const std::string& channel, int opt) {
    
    json msg = {
      {"method", "unsubscribe"},
      {"params", {
        {"channel", channel},
        {"symbol", {symbol}},
      }}
    };

    if (opt != -1) {
      if (channel == "ohlc") msg["params"]["interval"] = opt;
      if (channel == "book") msg["params"]["depth"] = opt;
    }

    return msg;
  }
};
