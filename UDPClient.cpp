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

    std::vector<std::byte> received = {};
    system::error_code ec;
    size_t receivedSize;

    try {
        std::cout << "Receive started!\n";
        asio::mutable_buffer receiveBuffer = asio::buffer(received, CLIENT_BUFFER_SIZE);
        receivedSize = clientSocket.receive_from(receiveBuffer, serverEndpoint, 0, ec);
        std::cout << "Buffer size (asio::buffer_size()): " << asio::buffer_size(receiveBuffer) << "\n";
        std::cout << "Buffer size (receiveBuffer.size()): " << receiveBuffer.size() << "\n";
        std::cout << "Receive ended! Size received: " << receivedSize << "\n";
    } catch (boost::system::system_error) {
        std::cerr << "Receive error!\n";
    }
    
    std::cout << "Error information: " << ec.message() << "\n";

    // Deserialize the Message object
    Message msg = Message();
    // msg = Message::deserialize(received);

    std::cout << "Message received" << std::endl;

    return msg;
}



