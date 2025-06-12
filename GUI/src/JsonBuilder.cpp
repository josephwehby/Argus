#include "JsonBuilder.hpp"
#include <iostream>

namespace JsonBuilder {
  json generateSubscribe(std::string symbol, const std::string& channel, int64_t id, std::string opt) {
    
    std::transform(symbol.cbegin(), symbol.cend(), symbol.begin(), [](char c) { return std::tolower(c); }); 
    std::string m = symbol + "@" + channel; 
        
    if (opt != "") {
      if (channel == "kline") {
        m = m + "_" + opt;
      } else if (channel == "depth") {
        m = m + "@" + opt;
      }
    }

    json msg = {
      {"method", "SUBSCRIBE"},
      {"params", 
        json::array({m})
      },
      {"id", id}
    };
    
    return msg;
  }

  json generateUnsubscribe(std::string symbol, const std::string& channel, int64_t id, std::string opt) {
    
    std::transform(symbol.cbegin(), symbol.cend(), symbol.begin(), [](char c) { return std::tolower(c); }); 
    std::string m = symbol + "@" + channel; 
    
    if (opt != "") {
      if (channel == "kline") {
        m = m + "_" + opt;
      } else if (channel == "depth") {
        m = m + "@" + opt;
      }
    }

    json msg = {
      {"method", "UNSUBSCRIBE"},
      {"params", 
        json::array({m})
      },
      {"id", id}
    };

    return msg;
  }
};
