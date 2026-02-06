#include "UDPReceiver.h"


using namespace boost;
using udp = asio::ip::udp;

// Constructor
UDPReceiver::UDPReceiver(unsigned short serverPort, unsigned short clientPort) 
    : clientSocket(ioc, udp::endpoint(udp::v4(), clientPort)),
    serverEndpoint(udp::v4(), serverPort) {
        ioc.run();
    }

// Receive a single serialized Message
Message UDPReceiver::receive() {

    std::vector<std::byte> received(CLIENT_BUFFER_SIZE);
    system::error_code ec;
    size_t receivedSize;

    try {
        std::cout << "Receive started!\n";
        asio::mutable_buffer receiveBuffer = asio::buffer(received.data(), received.size());
        std::cout << "Buffer size before(asio::buffer_size()): " << asio::buffer_size(receiveBuffer) << "\n";
        std::cout << "Buffer size before(receiveBuffer.size()): " << receiveBuffer.size() << "\n";
        receivedSize = clientSocket.receive_from(receiveBuffer, serverEndpoint, 0, ec); // Receive a message synchronously
        std::cout << "Buffer size (asio::buffer_size()): " << asio::buffer_size(receiveBuffer) << "\n";
        std::cout << "Buffer size (receiveBuffer.size()): " << receiveBuffer.size() << "\n";
        std::cout << "Receive ended! Size received: " << receivedSize << "\n";
    } catch (boost::system::system_error) {
        std::cerr << "Receive error!\n";
    }
    
    std::cout << "Error information: " << ec.message() << "\n";

    // Deserialize the buffer into a Message object
    Message msg = Message::deserialize(received, receivedSize);

    std::cout << "Message received" << std::endl;

    return msg;
}



