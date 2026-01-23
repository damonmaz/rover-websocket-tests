#include "UDPClient.h"


using namespace boost;
using udp = asio::ip::udp;

// Constructor
UDPClient::UDPClient(unsigned short port) 
    : clientSocket(ioc, udp::endpoint(udp::v4(), port)) {}

// Send a serialized Message to the UDP server
//void UDPClient::send(MessageQueue& queue) {
    //TODO
//}

// Receive a serialized Message from the UDP server
Message UDPClient::receive() {

    std::string received;
    clientSocket.async_receive(asio::buffer(received, 1024),
    [&](std::error_code ec, size_t bytesSent) {
        if (ec) {
            std::cerr << "Receive error: " << ec.message() << "\n";
        }
    });

    // Deserialize the Message object
    Message msg = Message::deserialize(received);

    return msg;
}