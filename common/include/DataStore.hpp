#pragma once

#include <mutex>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Level1.hpp"
#include "Trade.hpp"

class DataStore {
  public:
    DataStore();
    DataStore(const DataStore&) = delete;
    DataStore& operator=(const DataStore&) = delete;
    
    void setTicker(const std::string&, std::shared_ptr<Level1>);
    std::shared_ptr<Level1> getTicker(const std::string&) const;
    
    void setTrades(const std::string&, const std::vector<Trade>&);
    std::vector<Trade> getTrades(const std::string&) const;

  private:
    mutable std::mutex m;
    std::unordered_map<std::string, std::shared_ptr<Level1>> ticker_data;

    std::unordered_map<std::string, std::vector<Trade>> trade_data;
};
