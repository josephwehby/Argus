#include "HttpsClient.hpp"

HttpsClient::HttpsClient(std::shared_ptr<DataParser> dp_) : dp(dp_), cli(url.c_str()) {
  process_thread = std::thread(&HttpsClient::processLoop, this); 
}

HttpsClient::~HttpsClient() { 
  if (process_thread.joinable()) {
    process_thread.join();
  }
}

void HttpsClient::shutdown() {
  HttpsTask task;
  task.type = HttpsTaskType::Empty;
  events.push(task);
}

void HttpsClient::pushRequest(HttpsTask& task) {
  events.push(task);
}

void HttpsClient::processLoop() {
  bool run = true;

  while (run) {
    auto task = events.wait_and_pop();
    
    switch (task->type) {
      case HttpsTaskType::HistoricalChart:
        getHistoricalChart(task);
        break;
      case HttpsTaskType::OrderBook:
        getOrderBook(task);
        break;
      case HttpsTaskType::Empty:
        run = false;
        break;
      default:
        break;
    }
  }
}

void HttpsClient::getHistoricalChart(std::shared_ptr<HttpsTask> task) {
  std::string query = endpoint + "klines?symbol=" + task->symbol + "&interval=" + task->interval; 

  if (!task->start_time.empty()) {
    query += "&startTime=";
    query += task->start_time;
  }

  if (!task->end_time.empty()) {
    query += "&endTime=";
    query += task->end_time;
  }

  query += "&limit=" + task->limit;

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
  modified["s"] = task->symbol;
  modified["e"] = "kline-historic";
  modified["c"] = response;

  dp->pushData(modified);
}

void HttpsClient::getOrderBook(std::shared_ptr<HttpsTask> task) {
  std::string query = endpoint + "depth?symbol=" + task->symbol + "&limit=" + task->limit;
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
  response["s"] = task->symbol;
  response["e"] = "orderbook-snapshot";

  dp->pushData(response);
}
