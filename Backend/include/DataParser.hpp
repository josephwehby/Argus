#pragma once

#include <iostream>
#include <thread>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include <fstream>

#include "SafeQueue.hpp"
#include "Level1.hpp"
#include "DataStore.hpp"

using json = nlohmann::json;

class DataParser {
  public:
    DataParser();
    ~DataParser();
    void pushData(json); 
    
  private:
    std::thread t; 
    SafeQueue<json> data;
    DataStore ds; 
    void processLoop();
    void parseData(std::shared_ptr<json>);
    void parseTicker(std::shared_ptr<json>);
    void parseBook(std::shared_ptr<json>);
    void parseOHLC(std::shared_ptr<json>);
    void parseTrade(std::shared_ptr<json>);
};
