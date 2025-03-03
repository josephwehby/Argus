#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>
#include <time.h>
#include <stdlib.h>

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

  inline long long UTCToUnix(std::string utc_time) {
    std::tm tm{};

    auto pos = utc_time.find('.');

    if (pos != std::string::npos) {
      utc_time = utc_time.substr(0, pos);
    }
    
    std::istringstream ss(utc_time);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

    if (ss.fail()) {
      std::cerr << "Error parsing time" << std::endl;
      return -1; 
    }

    std::time_t tt = _mkgmtime(&tm);

    if (tt == -1) {
      std::cerr << "mktime failed" << std::endl;
      return -1;
    }
    std::chrono::sys_time<std::chrono::seconds> tp{std::chrono::seconds(tt)};
    long long unix_time = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();

    return unix_time;
  }

};
