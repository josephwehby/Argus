#include "HttpsClient.hpp"

HttpsClient::HttpsClient(std::shared_ptr<DataParser> dp_) : dp(dp_), cli(url.c_str()) {}

void HttpsClient::getHistoricalChart(const std::string& symbol, const std::string& interval, const std::string & limit) {
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

  std::cout << modified.dump() << std::endl;
  dp->pushData(modified);
}
