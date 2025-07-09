#include "OrderBookManager.hpp"

OrderBookManager::OrderBookManager(SubscriptionManager& sm_, HttpsClient& hc_, std::string& symbol_, int64_t id_) : 
  sm(sm_), hc(hc_), symbol(symbol_), id(id_) {
  event_channel_update += symbol;
  event_channel_snapshot += symbol;

  process_thread = std::thread(&OrderBookManager::processLoop, this);
}

OrderBookManager::~OrderBookManager() {
  if (process_thread.joinable()) {
    process_thread.join();
  }
  
  sm.unsubscribe(update_request);
  sm.unsubscribe(snapshot_request);
}

BookSnapshot OrderBookManager::getBookSnapshot() {
  std::lock_guard<std::mutex> lock(m);
  return book;
}

void OrderBookManager::init() {
  std::weak_ptr<OrderBookManager> self = shared_from_this();

  update_request = {
    symbol,
    channel,
    event_channel_update,
    id,
    [self](std::shared_ptr<IEvent> update){
      if (auto locked = self.lock()) {
        auto book_update = std::dynamic_pointer_cast<BookUpdate>(update);
        locked->updates.push(*book_update);
      }
    },
    speed
  };
  
  sm.subscribe(update_request);
  
  snapshot_request = {
    symbol,
    channel,
    event_channel_update,
    id,
    [self](std::shared_ptr<IEvent> update){
      if (auto locked = self.lock()) {
        auto book_snapshot= std::dynamic_pointer_cast<BookSnapshot>(update);
        locked->snapshots.push(*book_snapshot);
      }
    },
    std::nullopt
  };

  sm.subscribe(snapshot_request);
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
  bool sync = false;
  syncBook();
  while (true) {
    
    if (sync) {
      syncBook();
      sync = false;
    }

    auto event = updates.wait_and_pop();
    if (event->first_update == 0 && event->last_update == 0) {
      break;
    }

    bool success = applyUpdate(event);
    if (!success) {
      std::cout << "OB for " << symbol << " is out of sync." << std::endl;
      sync = true;
      updates.clear();
    }
  }
}

void OrderBookManager::syncBook() {
  std::shared_ptr<BookUpdate> update = updates.wait_and_front();
  BookSnapshot sync_book;
  
  do {
    HttpsTask task{HttpsTaskType::OrderBook, symbol, "0", limit};
    hc.pushRequest(task);
    sync_book = *(snapshots.wait_and_pop());
    if (sync_book.last_update == 0) return;
  } while (sync_book.last_update < update->first_update);
  
  while (true) {
    auto front = updates.wait_and_front();
    if (front->last_update <= sync_book.last_update) {
      updates.pop();
    } else {
      break;
    }
  }

  std::lock_guard<std::mutex> lock(m);
  book = sync_book;
}

bool OrderBookManager::applyUpdate(std::shared_ptr<BookUpdate> event) {
  std::lock_guard<std::mutex> lock(m);
  
  if (event->last_update < book.last_update) return true;
  if (event->first_update > book.last_update+1) return false;

  for (const auto& bid : event->bids) {
    if (bid.size == 0) {
      book.bids.erase(bid.price);
    } else {
      book.bids[bid.price] = bid.size;
    }
  }
  
  while (book.bids.size() > depth) {
    auto it = std::prev(book.bids.end());
    book.bids.erase(it);
  } 

  for (const auto& ask : event->asks) {
    if (ask.size == 0) {
      book.asks.erase(ask.price);
    } else {
      book.asks[ask.price] = ask.size;
    }
  }
  
  while (book.asks.size() > depth) {
    auto it = book.asks.begin();
    book.asks.erase(it);
  } 

  book.last_update = event->last_update;
  return true;
}
