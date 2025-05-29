#include "OrderBookManager.hpp"

OrderBookManager::OrderBookManager(std::string& token, std::shared_ptr<WebSocket> ws_) : symbol(token), ws(ws_) {
  json sub_msg = JsonBuilder::generateSubscribe(token, "depth", 1, speed);
  ws->subscribe(sub_msg); 
  hc->getOrderBook(symbol, limit);
}

OrderBookManager::~OrderBookManager() {
  json unsub_msg = JsonBuilder::generateUnsubscribe(token, "depth", 1, speed);
  ws->unsubscribe(unsub_msg);
}

void OrderBookManager::pushData(BookUpdate& update) {
  events.push(update);
}

void syncBook() {
  auto event = events.wait_and_pop();
   
}
