#pragma once

#include <iostream>
#include <thread>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>

#include "SafeQueue.hpp"
#include "Level1.hpp"
#include "Trade.hpp"
#include "DataStore.hpp"
#include "Candle.hpp"
#include "Utils.hpp"
#include "BookUpdate.hpp"

using json = nlohmann::json;

class DataParser {
  public:
    DataParser(std::shared_ptr<DataStore>);
    ~DataParser();
    void pushData(json); 
    void shutdown();

  private:
    std::thread process_thread; 
    SafeQueue<json> data;
    std::shared_ptr<DataStore> datastore; 
    
    void processLoop();
    void parseData(std::shared_ptr<json>);
    void parseTicker(std::shared_ptr<json>);
    void parseBook(std::shared_ptr<json>);
    void parseOHLC(std::shared_ptr<json>);
    void parseTrade(std::shared_ptr<json>);
};
