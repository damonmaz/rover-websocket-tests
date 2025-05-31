#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <memory>
#include <string>
#include <thread>
#include <functional>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession>
{
private:
    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer buffer_;
    std::function<void(const std::string&)> message_handler_;

public:
    explicit WebSocketSession(tcp::socket&& socket);
    
    void set_message_handler(std::function<void(const std::string&)> handler);
    void run();
    void send(const std::string& message);
    void close();

private:
    void on_accept(beast::error_code ec);
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void on_write(beast::error_code ec, std::size_t bytes_transferred);
};

class WebSocketServer
{
private:
    net::io_context ioc_;
    tcp::acceptor acceptor_;
    std::thread server_thread_;
    bool running_;
    std::function<void(const std::string&)> message_handler_;
    std::shared_ptr<WebSocketSession> current_session_;

public:
    explicit WebSocketServer(unsigned short port);
    ~WebSocketServer();
    
    void start();
    void stop();
    void set_message_handler(std::function<void(const std::string&)> handler);
    void send_message(const std::string& message);

private:
    void do_accept();
    void on_accept(beast::error_code ec, tcp::socket socket);
};

#endif // WEBSOCKET_SERVER_H