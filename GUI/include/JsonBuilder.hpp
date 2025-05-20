#pragma once

#include <algorithm>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace JsonBuilder {
  json generateSubscribe(std::string, const std::string&, int64_t, std::string = "");
  json generateUnsubscribe(std::string, const std::string&, int64_t, std::string = ""); 
};
