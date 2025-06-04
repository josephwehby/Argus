#include "OrderBookManager.hpp"

OrderBookManager::OrderBookManager(std::string& token, std::shared_ptr<WebSocket> ws_, std::shared_ptr<DataParser> dp_) : symbol(token), ws(ws_), dp(dp_), hc(dp_) {

  json sub_msg = JsonBuilder::generateSubscribe(token, "depth", 1, speed);
  ws->subscribe(sub_msg);

  process_thread = std::thread(&OrderBookManager::processLoop, this);
}

OrderBookManager::~OrderBookManager() {
  if (process_thread.joinable()) {
    process_thread.join();
  }

  json unsub_msg = JsonBuilder::generateUnsubscribe(token, "depth", 1, speed);
  ws->unsubscribe(unsub_msg);
}

void OrderBookManager::pushUpdate(BookUpdate& update) {
  std::lock_guard<std::mutex> lock(m);
  updates.push(update);
}

void OrderBookManager::pushSnapshot(BookSnapshot& snapshot) {
  std::lock_guard<std::mutex> lock(m);
  snapshots.push(snapshot);
}

BookSnapshot OrderBookManager::getBookSnapshot() {
  std::lock_guard<std::mutex> lock(m);
  return book;
}

void OrderBookManager::processLoop() {
  syncBook();
  while (true) {
    auto event = updates.wait_and_pop();
    if (event->is_null()) {
      break;
    }
    bool success = applyUpdate(event);
    if (!success) {
      std::cout << "OB for " << symbol << " is out of sync." << std::endl;
    }
  }
}

void syncBook() {
  std::lock_guard<std::mutex> lock(m);
  std::shared_ptr<BookUpdate> update = updates.wait_and_front();

  do {
    hc.getOrderBook(symbol, limit);
    book = snapshots.wait_and_pop();
  } while (book.last_update < update->first_update);
  
  while (updates.wait_and_front()->last_update <= book.last_update) updates.pop();
}

bool OrderBookManager::applyUpdate(std::shared_ptr<BookUpdate> event) {
  std::lock_guard<std::mutex> lock(m);
  if (event->last_update < book.last_update) return true;
  if (event->first_update > book.last_update) return false;

  for (const auto& bid :event->bids) {
    if (bid.size == 0) {
      book.bids.erase(bid.price);
    } else {
      book.bids[price] = size;
    }
  }

  for (const auto& ask : event->asks) {
    if (ask.size == 0) {
      book.asks.erase(ask.price);
    } else {
      book.asks[price] = size;
    }
  }
  
  book.last_update = event->last_update;
  return true;
}
