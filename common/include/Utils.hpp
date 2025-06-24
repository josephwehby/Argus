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

  inline std::string formatPriceFromString(std::string price) {
    auto pos = price.find('.');

    if (pos == std::string::npos) {
      return "";
    }

    int length = price.size() - pos - 1;
    while (length > 2) {
      price.pop_back();
      length--;
    }

    return price;
  }
  
  inline std::string formatSize(double size) {
    std::ostringstream oss;

    oss << std::fixed << std::setprecision(6) << size;

    return oss.str();
  }

  inline std::string formatSizeFromString(std::string size) {
    auto pos = size.find('.');

    if (pos == std::string::npos) {
      return "";
    }

    int length = size.size() - pos - 1;
    while (length > 5) {
      size.pop_back();
      length--;
    }

    return size;
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
  
  
  inline std::string formatMilliTime(long long milliseconds_time) {
    time_t seconds = static_cast<time_t> (milliseconds_time/1000);

    std::tm local_time{};
    if (localtime_s(&local_time, &seconds) != 0) {
      std::cerr << "local time conversion failed" << std::endl;
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
    
    return static_cast<long long>(utc_time);
  }

  inline long long getCandleDurationInSeconds(const std::string& time_frame) {

    if (time_frame == "1m") return 60;
    if (time_frame == "5m") return 300;
    if (time_frame == "15m") return 900;
    if (time_frame == "1h") return 3600;
    if (time_frame == "1d") return 86400;

    return 0;
  }

};
