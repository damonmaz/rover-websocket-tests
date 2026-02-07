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
void UDPHandler::receive(MessageQueue& queue) {

    asio::mutable_buffer receiveBuffer = asio::buffer(received.data(), received.size());

    mySocket.async_receive_from(receiveBuffer, theirEndpoint, // Receive a message asynchronously
        [&](std::error_code ec, size_t bytesReceived) {
        // Completion handler - called upon receive completion
        if (ec) {
            std::cerr << "Receive error: " << ec.message() << "\n";
        }

        if (bytesReceived != 0) {
            // Deserialize the buffer into a Message object
            Message msg = Message::deserialize(received, bytesReceived);
            isReceiving = false;
            msg.printMessage(); // Print message received

            // TEMP: testing
            if (msg.getFormat() != MESSAGE_FORMAT_GENERIC){
                queue.push(Message(Generic{1}));
            }
        } else {
            std::cout << "Received nothing \n";
            queue.push(Message(Generic{0}));
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

            // Send the serialized message to the client
            size_t returned = mySocket.send_to(msgBuffer, theirEndpoint);

            // Print the sent message
            Message out = Message::deserialize(serializedMsg, returned);
        } else if (!isReceiving) {
            UDPHandler::receive(queue);
        }
    }
}