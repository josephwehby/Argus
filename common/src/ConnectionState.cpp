#include "ConnectionState.hpp"

ConnectionState::ConnectionState(State initial_state) : state(initial_state) {}

State ConnectionState::getState() const {
  std::lock_guard<std::mutex> lock(m);
  State r = state;
  return r;
}

void ConnectionState::setState(State new_state) {
  std::lock_guard<std::mutex> lock(m);
  state = new_state;
}
