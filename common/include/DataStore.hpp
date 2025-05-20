#pragma once

#include <mutex>
#include <memory>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <utility>

#include "Level1.hpp"
#include "Trade.hpp"
#include "BookUpdate.hpp"
#include "Candle.hpp"

class DataStore {
  public:
    DataStore();
    ~DataStore();
    DataStore(const DataStore&) = delete;
    DataStore& operator=(const DataStore&) = delete;
        
    void setTicker(const std::string&, const Level1&);
    Level1 getTicker(const std::string&) const;
    
    void setTrade(const std::string&, const Trade&);
    std::vector<Trade> getTrades(const std::string&);

    void setBook(const std::string&, const BookUpdate&);
    BookUpdate getBook(const std::string&);
    
    void setCandle(const std::string&, const Candle&);
    std::vector<Candle> getCandles(const std::string&);
  private:
    mutable std::mutex m;
    std::unordered_map<std::string, Level1> ticker_data;
    std::unordered_map<std::string, std::vector<Trade>> trade_data;
    std::unordered_map<std::string, BookUpdate> book_data;
    std::unordered_map<std::string, std::vector<Candle>> candle_data;
};
