#include "OrderBookManager.hpp"

OrderBookManager::OrderBookManager(std::shared_ptr<WebSocket> ws_, std::shared_ptr<EventBus> eb_, std::shared_ptr<HttpsClient> hc_, std::string& symbol_, int64_t id_) : 
  symbol(symbol_), id(id_), ws(ws_), eb(eb_), hc(hc_) {
  std::cout << id << std::endl;  
  event_channel_update += symbol;
  event_channel_snapshot += symbol;

  json sub_msg = JsonBuilder::generateSubscribe(symbol, "depth", id, speed);
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

  json unsub_msg = JsonBuilder::generateUnsubscribe(symbol, "depth", id, speed);
  ws->unsubscribe(unsub_msg);

  eb->unsubscribe(event_channel_update, id);
  eb->unsubscribe(event_channel_snapshot, id);
}

BookSnapshot OrderBookManager::getBookSnapshot() {
  std::lock_guard<std::mutex> lock(m);
  return book;
}

void OrderBookManager::shutdown() {
  BookUpdate stop;
  stop.first_update = 0;
  stop.last_update = 0;
  updates.push(stop);

  BookSnapshot end;
  end.last_update = 0;
  snapshots.push(end);
}

void OrderBookManager::processLoop() {
  syncBook();
  while (true) {
    auto event = updates.wait_and_pop();
    if (event->first_update == 0 && event->last_update == 0) {
      break;
    }
    std::cout << "event" << std::endl;
    bool success = applyUpdate(event);
    if (!success) {
      std::cout << "OB for " << symbol << " is out of sync." << std::endl;
    }
  }
}

void OrderBookManager::syncBook() {
  std::shared_ptr<BookUpdate> update = updates.wait_and_front();
  BookSnapshot sync_book;
  
  std::cout << "sync" << std::endl;

  do {
    HttpsTask task{HttpsTaskType::OrderBook, symbol, "0", limit};
    hc->pushRequest(task);
    sync_book = *(snapshots.wait_and_pop());
    if (sync_book.last_update == 0) return;
  } while (sync_book.last_update < update->first_update);
  
  std::cout << "removing events..." << std::endl;

  while (true) {
    auto front = updates.wait_and_front();
    if (front->last_update <= sync_book.last_update) {
      std::cout << "[SYNC] DROPPING " << front->last_update << " " << sync_book.last_update << std::endl;
      updates.pop();
    } else {
      break;
    }
  }

  std::lock_guard<std::mutex> lock(m);
  book = sync_book;

  std::cout << "done syncing" << std::endl;
}

bool OrderBookManager::applyUpdate(std::shared_ptr<BookUpdate> event) {
  std::lock_guard<std::mutex> lock(m);
  std::cout << event->last_update << " " << event->first_update << " " << book.last_update << std::endl;
  if (event->last_update < book.last_update) return true;
  if (event->first_update > book.last_update) return false;

  std::cout << "applying update" << std::endl;
  
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
