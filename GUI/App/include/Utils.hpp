#pragma once

#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>

namespace Utils {

  inline std::string formatPrice(double price) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << price;

    return oss.str();
  }
  
  // converts it to central time. will adjust for other time zones later
  inline std::string formatTime(const std::string datetime) {
    int pos = datetime.find("T");

    std::string time = datetime.substr(pos+1, datetime.size() - pos - 2);
    
    pos = time.find(".");
    time = time.substr(0, pos); 

    int hour = std::stoi(time.substr(0, 2));
    hour = (hour - 6 + 24) % 24;

    std::string h = std::to_string(hour);

    if (h.size() < 2) {
      time[0] = '0';
      time[1] = h[0];
    } else {
      time[0] = h[0];
      time[1] = h[1];
    }
    
    return time;
  }
};
