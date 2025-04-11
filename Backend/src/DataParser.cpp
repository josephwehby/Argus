#include "DataParser.hpp"

DataParser::DataParser(DataStore& ds) : datastore(ds) {
  process_thread = std::thread(&DataParser::processLoop, this); 
}

DataParser::~DataParser() {
  data.push(nullptr);
  if (process_thread.joinable()) {
    process_thread.join();
  }
}

// may need to use move semantics later on to reduce the copies but ill try this for now
void DataParser::pushData(json new_data) {
  data.push(new_data);
}

void DataParser::processLoop() {
  while (true) {
    auto next_item = data.wait_and_pop();
    if (next_item == nullptr) break;
    parseData(next_item);
  }
}

void DataParser::parseData(std::shared_ptr<json> item) {

  if (!item->contains("channel")) {
    std::cout << "[!] subscribe message: ignoring" << std::endl;
    return;
  }

  std::string channel = item->at("channel");
  if (channel == "heartbeat" || channel == "status") {
    std::cout << "[!] heartbeat or status channel: ignoring" << std::endl; 
    return;
  }
  
  if (channel == "ticker") {
    parseTicker(item);    
  } else if (channel == "book") {

  } else if (channel == "ohlc") {

  } else if (channel == "trade") {

  } else {
    return;
  }
}

void DataParser::parseTicker(std::shared_ptr<json> item) {
  
  std::string symbol = item->at("data")[0]["symbol"];
  
  double best_bid = item->at("data")[0]["bid"];
  double best_ask = item->at("data")[0]["ask"];

  double best_bid_size = item->at("data")[0]["bid_qty"];
  double best_ask_size = item->at("data")[0]["ask_qty"];

  double price_change = item->at("data")[0]["change"];
  double percent_change = item->at("data")[0]["change_pct"];

  double last_price = item->at("data")[0]["last"];
  
  std::cout << "[!] " << symbol << " "<< " " << best_bid << " " << best_ask << " " << best_bid_size << " " << best_ask_size << std::endl;
  
  std::shared_ptr<Level1> ld = std::make_shared<Level1>(best_bid, best_ask, best_bid_size, best_ask_size, 
      price_change, percent_change, last_price);
  
  datastore.setTicker(symbol, ld);
}
