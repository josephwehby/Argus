#pragma once

#include <mutex>
#include <iostream>

enum class State { CONNECTING, CONNECTED, CLOSED };

class ConnectionState {
  public:
    ConnectionState() = default;
    ~ConnectionState();
    ConnectionState(State);

    State getState() const;
    void setState(State);

  private:
    State state;
    mutable std::mutex m;
};
