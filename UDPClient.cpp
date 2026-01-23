#include "UDPClient.h"


using namespace boost;
using udp = asio::ip::udp;

// Constructor
UDPClient::UDPClient(unsigned short serverPort, unsigned short clientPort) 
    : clientSocket(ioc, udp::endpoint(udp::v4(), clientPort)),
    serverEndpoint(udp::v4(), serverPort) {
        ioc.run();
    }

// Send a serialized Message to the UDP server
//void UDPClient::send(MessageQueue& queue) {
    //TODO
//}

// Receive a serialized Message from the UDP server
Message UDPClient::receive() {

    std::string received;
    try {
        std::cout << "Receive started!\n";
        clientSocket.receive_from(asio::buffer(received, 1024), serverEndpoint);
        std::cout << "Receive ended!\n";
    } catch (boost::system::system_error) {
        std::cerr << "Receive error!\n";
    }

    // Deserialize the Message object
    Message msg = Message::deserialize(received);

    return msg;
}