#pragma once

#include <iostream>
#include <thread>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <vector>

#include "SafeQueue.hpp"
#include "Level1.hpp"
#include "Trade.hpp"
#include "DataStore.hpp"
#include "Utils.hpp"
#include "BookUpdate.hpp"

using json = nlohmann::json;

class DataParser {
  public:
    DataParser(DataStore&);
    ~DataParser();
    void pushData(json); 
    
  private:
    std::thread process_thread; 
    SafeQueue<json> data;
    DataStore& datastore; 

    void processLoop();
    void parseData(std::shared_ptr<json>);
    void parseTicker(std::shared_ptr<json>);
    void parseBook(std::shared_ptr<json>);
    void parseOHLC(std::shared_ptr<json>);
    void parseTrade(std::shared_ptr<json>);
};
