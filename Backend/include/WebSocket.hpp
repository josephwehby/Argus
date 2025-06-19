#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <future>
#include <stdexcept>
#include <exception>
#include <deque>

#include "DataParser.hpp"
#include "DataStore.hpp"
#include "ConnectionState.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;
using json = nlohmann::json;

class WebSocket : public std::enable_shared_from_this<WebSocket> {
  public:
    explicit WebSocket(net::io_context&, ssl::context&, std::shared_ptr<DataParser>, std::shared_ptr<ConnectionState>);
    ~WebSocket();
    void fail(beast::error_code, char const*);
    void connect();
    void subscribe(json&);
    void unsubscribe(json&);
    void close();
    void waitClose();
    bool isOpen() const;
  private:
    void onResolve(beast::error_code, tcp::resolver::results_type);
    void onConnect(beast::error_code, tcp::resolver::results_type::endpoint_type);
    void onHandshake(beast::error_code);
    void onSSLHandshake(beast::error_code);
    void onRead(beast::error_code, std::size_t);
    void doRead();
    void send(json&);
    void onWrite(beast::error_code, std::size_t);
    void onClose(beast::error_code);

    std::string m_host = "data-stream.binance.vision";
    std::string m_port = "9443";
    
    std::shared_ptr<DataParser> dp;
    std::shared_ptr<ConnectionState> cs;

    tcp::resolver m_resolver;
    ssl::context m_ssl_ctx {ssl::context::tlsv12_client};
    websocket::stream<ssl::stream<beast::tcp_stream>> m_ws;
    beast::flat_buffer m_buffer;
    std::deque<std::shared_ptr<const std::string>> messages;

    std::promise<void> close_promise;
    std::future<void> close_future;
};
