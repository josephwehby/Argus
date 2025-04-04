#include "DataParser.hpp"

DataParser::DataParser() {

  // for now we are just going to read json from a file for testing purposes. 
  // eventually it will receive data from the websocket

  std::ifstream f{"../input.txt"}; 
    
  if (!f) {
    std::cout << "error " << std::endl;
  }

  std::shared_ptr<json> j = std::make_shared<json>(json::parse(f));
  parseData(j);
}

DataParser::~DataParser() {
  if (t.joinable()) {
    t.join();
  }
}

// may need to use move semantics later on to reduce the copies but ill try this for now
void DataParser::pushData(json new_data) {
  data.push(new_data);
}

void DataParser::processLoop() {
  while (true) {
    if (!data.empty()) {
      auto next_item = data.pop();
      parseData(next_item);
    }
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
  
  std::cout << "last price " << last_price << std::endl;
  std::cout << "bid " << best_bid << " " << best_bid_size << std::endl;
  std::cout << "ask " << best_ask << " " << best_ask_size << std::endl;
  std::cout << "price change " << price_change << std::endl;
  std::cout << "percent change " << percent_change << std::endl;

  std::cout << item->dump() << std::endl;
}
