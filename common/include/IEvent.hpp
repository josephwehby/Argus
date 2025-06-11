#pragma once

#include <string>

struct IEvent {
  virtual ~IEvent() = default;
  std::string channel;
};
