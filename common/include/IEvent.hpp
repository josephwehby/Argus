#pragma once

#include <string>
#include <functional>
#include <memory>


struct IEvent {
  virtual ~IEvent() = default;
  std::string channel;
};

using EventCallback = std::function<void(std::shared_ptr<IEvent>)>;
