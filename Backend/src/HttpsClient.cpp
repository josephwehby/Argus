#include "HttpsClient.hpp"

HttpsClient::HttpsClient() : cli(url.c_str()) {}

void HttpsClient::getHistoricalChart(const std::string& symbol, const std::string& interval, const std::string & limit) {
  std::string query = endpoint + "klines?symbol=" + symbol + "&interval=" + interval + "limit=" + limit;
  auto res = cli.Get(query);
  
  if (res->status != StatusCode::OK_200) {
    auto error = res.error();
    std::cerr << "HTTPS Error: " << httplib::to_string(error) << std::endl;
    return;
  }

  json response = json::parse(res->body);  
  std::cout << response.dump() << std::endl;
}
