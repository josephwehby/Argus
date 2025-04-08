#include <mutex>
#include <unordered_map>
#include <memory>

#include "Level1.hpp"

class DataStore {
  public:
    DataStore();
    std::shared_ptr<Level1> getTicker(const std::string&) const;
    void setTicker(const std::string&, std::shared_ptr<Level1>);

  private:
    mutable std::mutex m;
    std::unordered_map<std::string, std::shared_ptr<Level1>> ticker_data;
};
