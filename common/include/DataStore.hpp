#include <mutex>
#include <unordered_map>
#include "Level1.hpp"

class DataStore {
  public:
    DataStore();
    std::shared_ptr<Level1> getTicker(const std::string&);
    void setTicker(std::shared_ptr<Level1>);

  private:
    std::unordered_map<std::string, std::shared_ptr<Level1>> ticker_data;
};
