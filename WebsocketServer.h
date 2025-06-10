#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include "Message.h" 
#include <sstream>   


class WebSocketServer {
public:
    WebSocketServer(unsigned short port);
    void run();

private:
    void accept_connections();
    void handle_session(boost::asio::ip::tcp::socket socket);

    boost::asio::io_context ioc_;
    boost::asio::ip::tcp::acceptor acceptor_;
};
