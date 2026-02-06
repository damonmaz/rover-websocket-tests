#include "UDPServer.h"


using namespace boost;
using udp = asio::ip::udp;

// Constructor
UDPServer::UDPServer(unsigned short serverPort, unsigned short clientPort) 
    : serverSocket(ioc, udp::endpoint(udp::v4(), serverPort)), 
    clientEndpoint(udp::v4(), clientPort) {}


// Prepare the UDP sender
void UDPServer::run(MessageQueue& queue) {;
    std::thread(&UDPServer::handle_session, this, std::ref(queue)).detach();
    
    // Create some fake asio tasks - prevent the context from finishing
    asio::io_context::work idleWork(ioc);
    ioc.run();
}

// Continuously send messages from the queue over UDP
void UDPServer::handle_session(MessageQueue& queue) {
    while(true) {
        // Pop the next message from the queue (blocks if empty)
        Message msg = queue.pop();

        // Serialize Message object to a byte vector
        std::vector<std::byte> serializedMsg = msg.serialize();
        asio::mutable_buffer msgBuffer = asio::buffer(serializedMsg);
        std::cout << "Size prepared for sending: " << msgBuffer.size() << "\n";

        // Send the serialized message to the client
        size_t returned = serverSocket.send_to(msgBuffer, clientEndpoint);
        std::cout << "Size actually sent: " << returned << "\n";

        // Print the sent message
        Message out = Message::deserialize(serializedMsg, returned);
        std::cout << "Deserialized Message Type: " << out.getFormat() << std::endl;
    }
}