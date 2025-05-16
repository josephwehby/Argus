#include "JsonBuilder.hpp"

namespace JsonBuilder {
  json generateSubscribe(std::string symbol, const std::string& channel, int64_t id, int opt) {
    
    std::transform(symbol.cbegin(), symbol.cend(), symbol.begin(), [](char c) { return std::tolower(c); }); 
    std::string m = symbol + "@" + channel; 
    
    json msg = {
      {"method", "SUBSCRIBE"},
      {"params", 
        json::array({m})
      },
      {"id", id}
    };
    
    if (opt != -1) {
      if (channel == "ohlc") msg["params"]["interval"] = opt;
      if (channel == "book") msg["params"]["depth"] = opt;
    }
    
    return msg;
  }

  json generateUnsubscribe(std::string symbol, const std::string& channel, int64_t id, int opt) {
    
    std::transform(symbol.cbegin(), symbol.cend(), symbol.begin(), [](char c) { return std::tolower(c); }); 
    std::string m = symbol + "@" + channel; 
    
    json msg = {
      {"method", "UNSUBSCRIBE"},
      {"params", 
        json::array({m})
      },
      {"id", id}
    };

    if (opt != -1) {
      if (channel == "ohlc") msg["params"]["interval"] = opt;
      if (channel == "book") msg["params"]["depth"] = opt;
    }

    return msg;
  }
};
