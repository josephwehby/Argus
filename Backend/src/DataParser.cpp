#include "DataParser.hpp"

DataParser::DataParser(DataStore& ds) : datastore(ds) {
  process_thread = std::thread(&DataParser::processLoop, this); 
}

DataParser::~DataParser() {
  std::cout << "Desctrucotr called" << std::endl;
  data.push(nullptr);
  if (process_thread.joinable()) {
    std::cout << "joined" << std::endl;
    process_thread.join();
  }

  std::cout << "ds ran" << std::endl;
}

// may need to use move semantics later on to reduce the copies but ill try this for now
void DataParser::pushData(json new_data) {
  data.push(new_data);
}

void DataParser::processLoop() {
  while (true) {
    auto next_item = data.wait_and_pop();
    if (next_item == nullptr) {
      std::cout << "nullptr. time to exit" << std::endl;
      break;
    }
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
    parseBook(item);
  } else if (channel == "ohlc") {

  } else if (channel == "trade") {
    parseTrade(item);
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
  
  std::shared_ptr<Level1> ld = std::make_shared<Level1>(best_bid, best_ask, best_bid_size, best_ask_size, 
      price_change, percent_change, last_price);
  
  datastore.setTicker(symbol, ld);
}

void DataParser::parseBook(std::shared_ptr<json> item) {
  std::string symbol = item->at("data")[0]["symbol"];
  
  BookUpdate book_update;

  for (const auto& bid : (*item)["data"][0]["bids"]) {
    book_update.bids.emplace_back(bid["price"], bid["qty"]); 
  }
  
  for (const auto& ask: (*item)["data"][0]["asks"]) {
    book_update.asks.emplace_back(ask["price"], ask["qty"]); 
  }

  datastore.setBook(symbol, book_update);
}

void DataParser::parseTrade(std::shared_ptr<json> item) {
  std::string symbol = item->at("data")[0]["symbol"];
  
  std::vector<Trade> trades;
  for (const auto trade : item->at("data")) {
    TradeType type = (trade["ord_type"] == "limit") ? TradeType::Limit : TradeType::Market;
    TradeSide side = (trade["side"] == "buy") ? TradeSide::Buy : TradeSide::Sell;
    trades.emplace_back(type, side, Utils::formatPrice(trade["price"]), Utils::formatSize(trade["qty"]), Utils::formatTime(trade["timestamp"])); 
  }

  datastore.setTrades(symbol, trades);
}
