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
  
  inline std::string formatSize(double size) {
    std::ostringstream oss;

    oss << std::fixed << std::setprecision(6) << size;

    return oss.str();
  }

  inline std::string formatTime(const std::string datetime) {
    std::string trim = datetime.substr(0, datetime.find('.'));

    std::tm tm{};

    std::istringstream ss(trim);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

    if (ss.fail()) {
      std::cerr << "stringstream failed when converting time" << std::endl;
      return "";
    }

    std::time_t utc_time = _mkgmtime(&tm);

    std::tm local_time{};
    if (localtime_s(&local_time, &utc_time) != 0) {
      std::cerr << "local time failed" << std::endl;
      return "";
    }

    std::ostringstream oss;
    oss << std::put_time(&local_time, "%H:%M:%S");
    return oss.str();
  }

  inline long long UTCToUnix(std::string datetime) {
    std::string trim = datetime.substr(0, datetime.find('.'));

    std::tm tm{};

    std::istringstream ss(trim);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

    if (ss.fail()) {
      std::cerr << "stringstream failed when converting time" << std::endl;
      return -1;
    }

    std::time_t utc_time = _mkgmtime(&tm);

    std::tm local_time{};
    if (localtime_s(&local_time, &utc_time) != 0) {
      std::cerr << "local time failed" << std::endl;
      return -1;
    }

    std::time_t unix_time = std::mktime(&local_time);

    return static_cast<long long>(unix_time);
  }

};
