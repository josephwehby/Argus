#include "DataParser.hpp"

DataParser::DataParser(std::shared_ptr<DataStore> ds, std::shared_ptr<ConnectionState> cs_) : datastore(ds), cs(cs_) {
  process_thread = std::thread(&DataParser::processLoop, this); 
}

DataParser::~DataParser() {
  if (process_thread.joinable()) {
    process_thread.join();
  }

  datastore.reset();
}

void DataParser::shutdown() {
  data.push(nullptr);
}

// may need to use move semantics later on to reduce the copies but ill try this for now
void DataParser::pushData(json new_data) {
  data.push(new_data);
}

void DataParser::processLoop() {
  while (true) {
    auto next_item = data.wait_and_pop();
    if (next_item->is_null()) {
      break;
    }
    parseData(next_item);
  }
}

void DataParser::parseData(std::shared_ptr<json> item) {
  /*
  if (!item->contains("channel")) {
    std::cout << "[!] subscribe message: ignoring" << std::endl;
        return;
  }

  if (channel == "heartbeat" || channel == "status") {
    if (channel == "status" && item->at("data")[0]["system"] == "online") {
      cs->setState(State::CONNECTED);
    }

    return;
  }
  */
  if (!item->contains("e")) {
    std::cout << "item does not contain market data" << std::endl;
    return;
  }

  std::string channel = item->at("e");
  
  if (channel == "24hrTicker") {
    parseTicker(item);    
  } else if (channel == "book") {
    parseBook(item);
  } else if (channel == "kline") {
    parseOHLC(item);
  } else if (channel == "trade") {
    parseTrade(item);
  } else {
    return;
  }
}

void DataParser::parseTicker(std::shared_ptr<json> item) {
  
  std::string symbol = item->at("s");
  double best_bid = std::stod(std::string(item->at("b")));
  double best_ask = std::stod(std::string(item->at("a")));

  double best_bid_size = std::stod(std::string(item->at("B")));
  double best_ask_size = std::stod(std::string(item->at("A")));

  double price_change = std::stod(std::string(item->at("p")));
  double percent_change = std::stod(std::string(item->at("P")));

  double last_price = std::stod(std::string(item->at("c")));

  double high = std::stod(std::string(item->at("h")));
  double low = std::stod(std::string(item->at("l")));
  double volume = std::stod(std::string(item->at("v")));
  
  // i dont think this needs to be a shared ptr. can remove this later
  Level1 ld(best_bid, best_ask, best_bid_size, best_ask_size, 
      price_change, percent_change, last_price, high, low, volume);
  
  datastore->setTicker(symbol, ld);
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

  datastore->setBook(symbol, book_update);
}

void DataParser::parseOHLC(std::shared_ptr<json> item) {
  std::string symbol = item->at("s");
  
  double open = std::stod(std::string(item->at("k")["o"])); 
  double close = std::stod(std::string(item->at("k")["c"])); 
  double high = std::stod(std::string(item->at("k")["h"])); 
  double low = std::stod(std::string(item->at("k")["l"])); 
  double buy_volume = std::stod(std::string(item->at("k")["V"])); 
  double volume = std::stod(std::string(item->at("k")["v"])); 
  long long unix_time = item->at("k")["t"].get<long long>() / 1000;

  Candle candle(open, high, low, close, buy_volume, volume, 1, unix_time);
  datastore->setCandle(symbol, candle);
}

void DataParser::parseTrade(std::shared_ptr<json> item) {
  std::string symbol = item->at("s");
  
  TradeSide side = (item->at("m") == true) ? TradeSide::Sell: TradeSide::Buy;
  Trade trade(side, Utils::formatPriceFromString(item->at("p")), Utils::formatSizeFromString(item->at("q")), Utils::formatMilliTime(item->at("T"))); 
  datastore->setTrade(symbol, trade);
}
