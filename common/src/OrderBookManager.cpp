#include "OrderBookManager.hpp"

OrderBookManager::OrderBookManager(std::string& token, int64_t id_, std::shared_ptr<WebSocket> ws_, std::shared_ptr<EventBus> eb_, std::shared_ptr<HttpsClient> hc_) : symbol(token), id(id_), ws(ws_), eb(eb_), hc(hc_) {
  
  event_channel_update += symbol;
  event_channel_snapshot += symbol;

  json sub_msg = JsonBuilder::generateSubscribe(token, "depth", 1, speed);
  ws->subscribe(sub_msg);
  
  eb->subscribe(event_channel_update, id, [this](std::shared_ptr<IEvent> update){
    auto book_update = std::dynamic_pointer_cast<BookUpdate>(update);
    this->updates.push(*book_update);
  });
  
  eb->subscribe(event_channel_snapshot, id, [this](std::shared_ptr<IEvent> update){
    auto book_snapshot= std::dynamic_pointer_cast<BookSnapshot>(update);
    this->snapshots.push(*book_snapshot);
  });

  process_thread = std::thread(&OrderBookManager::processLoop, this);
}

OrderBookManager::~OrderBookManager() {
  if (process_thread.joinable()) {
    process_thread.join();
  }

  json unsub_msg = JsonBuilder::generateUnsubscribe(token, "depth", 1, speed);
  ws->unsubscribe(unsub_msg);

  eb->unsubscribe(event_channel_update, id);
  eb->unsubscribe(event_channel_snapshot, id);
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
    HttpsTask task{HttpsTaskType::OrderBook, symbol, "0", "20"};
    hc->pushRequest(task);
    book = snapshots.wait_and_pop();
  } while (book.last_update < update->first_update);
  
  while (updates.wait_and_front()->last_update <= book.last_update) updates.pop();
}

bool OrderBookManager::applyUpdate(std::shared_ptr<BookUpdate> event) {
  std::lock_guard<std::mutex> lock(m);
  if (event->last_update < book.last_update) return true;
  if (event->first_update > book.last_update) return false;

  for (const auto& bid : event->bids) {
    if (bid.size == 0) {
      book.bids.erase(bid.price);
    } else {
      book.bids[bid.price] = bid.size;
    }
  }

  for (const auto& ask : event->asks) {
    if (ask.size == 0) {
      book.asks.erase(ask.price);
    } else {
      book.asks[ask.price] = ask.size;
    }
  }
  
  book.last_update = event->last_update;
  return true;
}
