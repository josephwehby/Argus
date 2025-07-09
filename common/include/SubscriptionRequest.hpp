#pragma once

#include <optional>

#include "IEvent.hpp"

struct SubscriptionRequest {
  std::string symbol;
  std::string channel;
  std::string event_channel;
  int64_t id;
  
  EventCallback callback;

  std::optional<std::string> params;
};
