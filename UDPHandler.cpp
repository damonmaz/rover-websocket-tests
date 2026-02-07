#include "UDPHandler.h"

using namespace boost;
using udp = asio::ip::udp;

// Constructor
UDPHandler::UDPHandler(unsigned short myPort, unsigned short theirPort)
    : mySocket(ioc, udp::endpoint(udp::v4(), myPort)),
    theirEndpoint(udp::v4(), theirPort) { 
        received.resize(RECEIVE_BUFFER_SIZE);
    }

// Prepare the UDP sender
void UDPHandler::run(MessageQueue& queue) {;
    std::thread(&UDPHandler::handle_session, this, std::ref(queue)).detach();
    
    // Create some fake asio tasks - prevent the context from finishing
    asio::io_context::work idleWork(ioc);
    ioc.run();
}

// Receive a single serialized Message
void UDPHandler::receive() {
    //std::cout << "Receive started!\n";
    asio::mutable_buffer receiveBuffer = asio::buffer(received.data(), received.size());
    //std::cout << "Buffer size before(asio::buffer_size()): " << asio::buffer_size(receiveBuffer) << "\n";
    //std::cout << "Buffer size before(receiveBuffer.size()): " << receiveBuffer.size() << "\n";
    mySocket.async_receive_from(receiveBuffer, theirEndpoint, // Receive a message asynchronously
        [&](std::error_code ec, size_t bytesReceived) {
        // Completion handler - called upon receive completion
        if (ec) {
            std::cerr << "Receive error: " << ec.message() << "\n";
        }

        if (bytesReceived != 0) {
            //std::cout << "Buffer size (asio::buffer_size()): " << asio::buffer_size(receiveBuffer) << "\n";
            //std::cout << "Buffer size (receiveBuffer.size()): " << receiveBuffer.size() << "\n";
            //std::cout << "Receive ended! Size received: " << receivedSize << "\n";

            // Deserialize the buffer into a Message object
            Message msg = Message::deserialize(received, bytesReceived);
            std::cout << "Message received" << std::endl;
            isReceiving = false;
            msg.printMessage(); // Print message received
        } else {
            std::cout << "Received nothing \n";
        }

        }); 

    isReceiving = true;
}

// Continuously send messages from the queue over UDP
void UDPHandler::handle_session(MessageQueue& queue) {


    while(true) {
        if (!queue.empty()){
            // Pop the next message from the queue (blocks if empty)
            Message msg = queue.pop();

            // Serialize Message object to a byte vector
            std::vector<std::byte> serializedMsg = msg.serialize();
            asio::mutable_buffer msgBuffer = asio::buffer(serializedMsg);
            std::cout << "Size prepared for sending: " << msgBuffer.size() << "\n";

            // Send the serialized message to the client
            size_t returned = mySocket.send_to(msgBuffer, theirEndpoint);
            std::cout << "Size actually sent: " << returned << "\n";

            // Print the sent message
            Message out = Message::deserialize(serializedMsg, returned);
            std::cout << "Deserialized Message Type: " << out.getFormat() << std::endl;
        } else if (!isReceiving) {
            UDPHandler::receive();
        }
    }
}