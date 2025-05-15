#pragma once

#include <algorithm>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace JsonBuilder {
  json generateSubscribe(std::string, const std::string&, int64_t, int opt = -1);
  json generateUnsubscribe(std::string, const std::string&, int64_t, int opt = -1); 
};
