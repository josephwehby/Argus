#include "DataParser.hpp"

DataParser::DataParser(EventBus& eb_) : eb(eb_) {
  process_thread = std::thread(&DataParser::processLoop, this); 
}

DataParser::~DataParser() {
}

void DataParser::shutdown() {
  data.push(nullptr);
  if (process_thread.joinable()) {
    process_thread.join();
  }
}

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
  if (!item->contains("e")) {
    std::cout << "item does not contain market data" << std::endl;
    return;
  }

  std::string channel = item->at("e");

  if (channel == "24hrTicker") {
    parseTicker(item); 
  } else if (channel == "depthUpdate") {
    parseBookUpdate(item);
  } else if (channel == "orderbook-snapshot") {
    parseBookSnapshot(item);
  } else if (channel == "kline") {
    parseOHLC(item);
  } else if (channel == "kline-historic") {
    parseOHLCHistoric(item);
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

  std::shared_ptr<Level1> ld = std::make_shared<Level1>(best_bid, best_ask, best_bid_size, best_ask_size, 
                                                        price_change, percent_change, last_price, high, low, volume);
  ld->channel = "LEVEL1:" + symbol;

  eb.publish(ld);
}

void DataParser::parseBookUpdate(std::shared_ptr<json> item) {
  std::string symbol = item->at("s");

  std::shared_ptr<BookUpdate> book_update = std::make_shared<BookUpdate>();
  book_update->first_update = item->at("U").get<unsigned long long>(); 
  book_update->last_update = item->at("u").get<unsigned long long>(); 

  for (const auto& bid : (*item)["b"]) {
    book_update->bids.emplace_back(std::stod(bid[0].get<std::string>()), std::stod(bid[1].get<std::string>()));
  }

  for (const auto& ask: (*item)["a"]) {
    book_update->asks.emplace_back(std::stod(ask[0].get<std::string>()), std::stod(ask[1].get<std::string>()));
  }

  book_update->channel = "BOOK_UPDATE:" + symbol;
  eb.publish(book_update);
}

void DataParser::parseBookSnapshot(std::shared_ptr<json> item) {
  std::string symbol = item->at("s");
  std::shared_ptr<BookSnapshot> book_snapshot = std::make_shared<BookSnapshot>();
  book_snapshot->last_update = item->at("lastUpdateId").get<unsigned long long>();

  for (const auto& bid : (*item)["bids"]) {
    book_snapshot->bids.emplace(std::stod(bid[0].get<std::string>()), std::stod(bid[1].get<std::string>()));
  }

  for (const auto& ask: (*item)["asks"]) {
    book_snapshot->asks.emplace(std::stod(ask[0].get<std::string>()), std::stod(ask[1].get<std::string>()));
  }

  book_snapshot->channel = "BOOK_SNAPSHOT:" + symbol;
  eb.publish(book_snapshot);
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
  long long event_time = item->at("E").get<long long>();

  std::shared_ptr<Candle> candle = std::make_shared<Candle>(open, high, low, close, buy_volume, volume, 1, unix_time, event_time);
  candle->channel = "CANDLE:" + symbol;
  eb.publish(candle);
}

void DataParser::parseOHLCHistoric(std::shared_ptr<json> item) {
  std::string symbol = item->at("s");
  std::shared_ptr<HistoricalCandles> historical_candles = std::make_shared<HistoricalCandles>();

  for (const auto& candle : item->at("c")) {
    long long unix_time = candle[0].get<long long>()/1000;
    double open = std::stod(candle[1].get<std::string>());
    double high = std::stod(candle[2].get<std::string>());
    double low = std::stod(candle[3].get<std::string>());
    double close = std::stod(candle[4].get<std::string>());
    double volume = std::stod(candle[5].get<std::string>());
    double buy_volume = std::stod(candle[9].get<std::string>());
    historical_candles->candles.emplace_back(open, high, low, close, buy_volume, volume, 1, unix_time, 0);
  }

  historical_candles->channel = "HISTORICAL_CANDLES:" + symbol;
  eb.publish(historical_candles);
}

void DataParser::parseTrade(std::shared_ptr<json> item) {
  std::string symbol = item->at("s");

  TradeSide side = (item->at("m") == true) ? TradeSide::Sell: TradeSide::Buy;
  std::shared_ptr<Trade> trade = std::make_shared<Trade>(side, Utils::formatPriceFromString(item->at("p")), Utils::formatSizeFromString(item->at("q")), Utils::formatMilliTime(item->at("T"))); 
  trade->channel = "TRADE:" + symbol;
  eb.publish(trade);
}
