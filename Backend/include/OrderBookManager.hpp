#pragma once

#include <iostream>
#include <thread>
#include <vector>

#include "BookUpdate.hpp"
#include "SafeQueue.hpp"
#include "HttpsClient.hpp"

class OrderBookManager {
  public:
    OrderBookManager();
  private:
    void processLoop();

    SafeQueue<BookUpdate> events;
};
