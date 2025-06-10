#include "WebsocketClient.h"
#include "Message.h" // Add this include

#include <iostream>

using namespace boost;
using tcp = asio::ip::tcp;
namespace websocket = beast::websocket;

WebSocketClient::WebSocketClient(const std::string& host, const std::string& port)
    : host_(host), port_(port), ws_(ioc_) {}

void WebSocketClient::connect() {
    tcp::resolver resolver(ioc_);
    auto const results = resolver.resolve(host_, port_);
    asio::connect(ws_.next_layer(), results.begin(), results.end());
    ws_.handshake(host_, "/");
}

void WebSocketClient::send(const std::string& message) {
    ws_.write(asio::buffer(message));
}

std::string WebSocketClient::receive() {
    beast::flat_buffer buffer;
    ws_.read(buffer);
    std::string received = beast::buffers_to_string(buffer.data());

    std::cout << "Received from server: " << received << std::endl;

    return received;
}

void WebSocketClient::close() {
    ws_.close(websocket::close_code::normal);
}