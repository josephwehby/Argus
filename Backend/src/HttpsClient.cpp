#include "HttpsClient.hpp"

HttpsClient::HttpsClient(std::shared_ptr<DataParser> dp_) : dp(dp_), cli(url.c_str()) {}

json HttpsClient::getHistoricalChart(const std::string& symbol, const std::string& interval, const std::string & limit) {
  std::string query = endpoint + "klines?symbol=" + symbol + "&interval=" + interval + "&limit=" + limit;
  auto res = cli.Get(query);
  
  if (res == nullptr) {
    std::cout << "nullptr" << std::endl;
    return;
  }

  if (res->status != httplib::StatusCode::OK_200) {
    auto error = res.error();
    std::cerr << "HTTPS Error: " << httplib::to_string(error) << std::endl;
    return;
  }

  json response = json::parse(res->body);  
  json modified;
  modified["s"] = symbol;
  modified["e"] = "kline-historic";
  modified["c"] = response;

  dp->pushData(modified);
}

json HttpsClient::getOrderBook(const std::string& symbol, const std::string& limit) {
  std::string query = endpoint + "depth?symbol=" + symbol + "&limit=" + limit;
  auto res = cli.Get(query);

  if (res == nullptr) {
    std::cerr << "nullptr error" << std::endl;
    return;
  }

  if (res->status != httplib::StatusCode::OK_200) {
    auto error = res.error();
    std::cerr << "HTTPS Error: " << httplib::to_string(error) << std::endl;
    return;
  }

  json response = json::parse(res->body);
  response["s"] = symbol;
  response["e"] = "orderbook-snapshot";

  dp->pushData(response);
}
