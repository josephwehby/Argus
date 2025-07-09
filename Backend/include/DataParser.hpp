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
#include "HistoricalCandles.hpp"
#include "Utils.hpp"
#include "BookUpdate.hpp"
#include "BookSnapshot.hpp"
#include "EventBus.hpp"

using json = nlohmann::json;

class DataParser {
  public:
    DataParser(EventBus&);
    ~DataParser();
    void pushData(json); 
    void shutdown();

  private:
    std::thread process_thread; 
    SafeQueue<json> data;
    
    EventBus& eb;

    void processLoop();
    void parseData(std::shared_ptr<json>);
    void parseTicker(std::shared_ptr<json>);
    void parseBookUpdate(std::shared_ptr<json>);
    void parseBookSnapshot(std::shared_ptr<json>);
    void parseOHLC(std::shared_ptr<json>);
    void parseOHLCHistoric(std::shared_ptr<json>);
    void parseTrade(std::shared_ptr<json>);
};
