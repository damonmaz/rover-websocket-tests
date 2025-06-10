#pragma once
#include <string>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include "Message.h" 
#include <iostream>


class WebSocketClient {
public:
    WebSocketClient(const std::string& host, const std::string& port);
    void connect();
    void send(const std::string& message);
    std::string receive();
    void close();

private:
    std::string host_;
    std::string port_;
    boost::asio::io_context ioc_;
    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
};
