#pragma once

#include <mutex>
#include <deque>
#include <condition_variable>
#include <stdexcept>
#include <iostream>

template <typename T>
class SafeQueue {
  public:
    SafeQueue() {}

    SafeQueue(const SafeQueue& other) {
      std::lock_guard<std::mutex> lock(other.m);
      data = other.data; 
    }

    SafeQueue& operator=(const SafeQueue&) = delete;
    
    bool empty() const {
      std::lock_guard<std::mutex> lock(m);
      return data.empty();
    }

    void push(T value) {
      std::lock_guard<std::mutex> lock(m);
      data.push_back(std::move(value));
      data_cond.notify_one();
    }

    std::shared_ptr<T> pop() {
      std::lock_guard<std::mutex> lock(m);
      
      if (data.empty()) {
        throw std::runtime_error("Deque is empty");
      }

      std::shared_ptr<T> const res = std::make_shared<T>(data.front());
      data.pop_front();
      return res;
    }

    std::shared_ptr<T> wait_and_pop() {

      std::unique_lock<std::mutex> lock(m);
      data_cond.wait(lock, [this]{ return !data.empty(); });
      
      std::shared_ptr<T> const res = std::make_shared<T>(data.front());
      data.pop_front();
      return res;

    }

    std::shared_ptr<T> wait_and_front() {
      std::unique_lock<std::mutex> lock(m);
      data_cond.wait(lock, [this]{ return !data.empty(); });
      
      std::shared_ptr<T> const res = std::make_shared<T>(data.front());
      return res;
    }

  private:
    std::deque<T> data;
    mutable std::mutex m;
    std::condition_variable data_cond; 
};
