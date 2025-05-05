#include <mutex>

enum State { CONNECTING, CONNECTED, CLOSED };

class ConnectionState {
  public:
    ConnectionState() = default;
    ConnectionState(State);

    State getState() const;
    void setState(State);

  private:
    State state;
    std::mutex m;
};
