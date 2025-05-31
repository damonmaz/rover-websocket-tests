#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <memory>
#include <string>
#include <thread>
#include <functional>
#include <atomic>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class WebSocketClient : public std::enable_shared_from_this<WebSocketClient>
{
private:
    tcp::resolver resolver_;
    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer buffer_;
    std::string host_;
    std::string port_;
    std::string target_;
    net::io_context& ioc_;
    std::function<void(const std::string&)> message_handler_;
    std::function<void()> connect_handler_;
    std::function<void()> disconnect_handler_;
    std::atomic<bool> connected_;

public:
    explicit WebSocketClient(net::io_context& ioc);
    
    void connect(const std::string& host, const std::string& port, const std::string& target = "/");
    void disconnect();
    void send(const std::string& message);
    bool is_connected() const;
    
    void set_message_handler(std::function<void(const std::string&)> handler);
    void set_connect_handler(std::function<void()> handler);
    void set_disconnect_handler(std::function<void()> handler);

private:
    void on_resolve(beast::error_code ec, tcp::resolver::results_type results);
    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);
    void on_handshake(beast::error_code ec);
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void on_write(beast::error_code ec, std::size_t bytes_transferred);
    void on_close(beast::error_code ec);
};

class WebSocketClientManager
{
private:
    net::io_context ioc_;
    std::thread client_thread_;
    std::shared_ptr<WebSocketClient> client_;
    bool running_;

public:
    WebSocketClientManager();
    ~WebSocketClientManager();
    
    void start();
    void stop();
    void connect(const std::string& host, const std::string& port, const std::string& target = "/");
    void disconnect();
    void send_message(const std::string& message);
    bool is_connected() const;
    
    void set_message_handler(std::function<void(const std::string&)> handler);
    void set_connect_handler(std::function<void()> handler);
    void set_disconnect_handler(std::function<void()> handler);
};

#endif