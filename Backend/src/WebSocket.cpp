#include "WebSocket.hpp"

void WebSocket::fail(beast::error_code ec, char const* what) {
    std::cerr << what << ": " << ec.message() << "\n";
    cs.setState(State::CLOSED);
}

WebSocket::WebSocket(net::io_context& ioc, ssl::context& ctx, DataParser& dp_, ConnectionState& cs_) 
  : m_resolver(net::make_strand(ioc)),
    m_ssl_ctx(std::move(ctx)),
    m_ws(net::make_strand(ioc), m_ssl_ctx),
    dp(dp_),
    cs(cs_) {
      
      //m_ssl_ctx = std::move(ctx);
}

WebSocket::~WebSocket() {
  std::cout << "websocket desturctor ran" << std::endl;
}

void WebSocket::connect() {
  m_resolver.async_resolve(m_host, m_port, beast::bind_front_handler(&WebSocket::onResolve, shared_from_this()));
}

void WebSocket::subscribe(json& subscribe_msg) {
  if (!m_ws.is_open()) {
    std::cout << "[!] Websocket is not open. Unable to unsubscribe." << std::endl;
    return;
  }
  
  send(subscribe_msg);
}

void WebSocket::unsubscribe(json& unsubscribe_msg) {
  if (!m_ws.is_open()) {
    return;
  }

  send(unsubscribe_msg);  
}

void WebSocket::close() {
  if (!m_ws.is_open()) return;

  close_promise = std::promise<void>();
  close_future = close_promise.get_future();

  m_ws.async_close(beast::websocket::close_code::normal,
      beast::bind_front_handler(&WebSocket::onClose, shared_from_this())
      );
}

void WebSocket::waitClose() {
  if (close_future.valid()) {
    try {
      close_future.wait();
      std::cout << "ws closed" << std::endl;
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}

bool WebSocket::isOpen() const {
  return m_ws.is_open();
}

void WebSocket::onResolve(beast::error_code ec, tcp::resolver::results_type results) {
  if (ec) {
    return fail(ec, "resolve");
  }
  
  beast::get_lowest_layer(m_ws).expires_after(std::chrono::seconds(wait_time));

  beast::get_lowest_layer(m_ws).async_connect(
      results,
      beast::bind_front_handler(
        &WebSocket::onConnect, shared_from_this())
      );

  std::cout << "resolved" << std::endl;
}

void WebSocket::onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep) {
  if (ec) {
    return fail(ec, "connect");
  }

  beast::get_lowest_layer(m_ws).expires_after(std::chrono::seconds(wait_time));
  
  if (!SSL_set_tlsext_host_name(m_ws.next_layer().native_handle(), m_host.c_str())) {
    ec = beast::error_code(static_cast<int>(::ERR_get_error()), net::error::get_ssl_category());
    return fail(ec, "connect");
  }

  m_host += ':' + std::to_string(ep.port());

  m_ws.next_layer().async_handshake(
      ssl::stream_base::client,
      beast::bind_front_handler(
        &WebSocket::onSSLHandshake, shared_from_this())
      );

  std::cout << "connect" << std::endl;
}

void WebSocket::onSSLHandshake(beast::error_code ec) {
  if (ec) {
    return fail(ec, "SSL");
  }

  beast::get_lowest_layer(m_ws).expires_never();

  m_ws.set_option(
      websocket::stream_base::timeout::suggested(
        beast::role_type::client));

   m_ws.set_option(websocket::stream_base::decorator(
    [](websocket::request_type& req) {
      req.set(http::field::host, "data-stream.binance.vision");
      req.set(http::field::user_agent,
        std::string(BOOST_BEAST_VERSION_STRING) +
        " websocket-client-async-ssl");
      }));

  m_ws.async_handshake(m_host, "/ws",
    beast::bind_front_handler(
    &WebSocket::onHandshake, shared_from_this()));

  std::cout << "handshake" << std::endl;
}

void WebSocket::onHandshake(beast::error_code ec) {
  if (ec) {
    return fail(ec, "handshake");
  }
  
  std::cout << "onHandsahke" << std::endl;
  cs.setState(State::CONNECTED);
  doRead();
}

void WebSocket::onRead(beast::error_code ec, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec) {
    return fail(ec, "on read");
  }
  
  std::string data = beast::buffers_to_string(m_buffer.data());
  json jsonData = json::parse(data);
  dp.pushData(jsonData);
  m_buffer.consume(m_buffer.size());

  doRead();
}

void WebSocket::doRead() {
  m_ws.async_read(m_buffer, 
      beast::bind_front_handler(&WebSocket::onRead, shared_from_this()));
}

void WebSocket::send(json& msg) {
  messages.push_back(std::make_shared<const std::string>(msg.dump()));
  
  if (messages.size() > 1) return;
  
  m_ws.async_write(net::buffer(*messages.front()),
      beast::bind_front_handler(&WebSocket::onWrite, shared_from_this()));
}

void WebSocket::onWrite(beast::error_code ec, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);
  if (ec) {
    return fail(ec, "on write");
  }

  messages.pop_front();

  if (!messages.empty()) {
    m_ws.async_write(net::buffer(*messages.front()),
      beast::bind_front_handler(&WebSocket::onWrite, shared_from_this()));
  }
}

void WebSocket::onClose(beast::error_code ec) {
  if (ec) {
    std::cout << "close error" << std::endl;
    close_promise.set_exception(std::make_exception_ptr(std::runtime_error("Error closing")));
    std::cout << ec.value() << " - " << ec.message() << std::endl;
    return fail(ec, "close");
  }
  
  std::cout << "websocket closed" << std::endl;
  close_promise.set_value();
}

