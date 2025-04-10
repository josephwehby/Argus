#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace JsonBuilder {
  json generateSubscribe(const std::string&, const std::string&, int opt = -1);
  json generateUnsubscribe(const std::string&, const std::string&, int interval = -1); 
};
