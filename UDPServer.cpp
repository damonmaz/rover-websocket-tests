#include "UDPServer.h"


using namespace boost;
using udp = asio::ip::udp;
//namespace websocket = beast::websocket;

// Constructor
UDPServer::UDPServer(unsigned short port) 
    : serverSocket(ioc, udp::endpoint(udp::v4(), port)) {}


// Run the WebSocket server, sending messages from the queue
void UDPServer::run(MessageQueue& queue) {
    // Use a resolver to get the client endpoint
    //udp::resolver resolver(ioc);
    //clientEndpoint = *resolver.resolve(udp::v4(), "base_server", "rover_client").begin();

    std::thread(&UDPServer::handle_session, this, std::ref(queue)).detach();
    ioc.run();
}

// Handle a single WebSocket session with a connected client
void UDPServer::handle_session(MessageQueue& queue) {
    while(true) {
        // Pop the next message from the queue (blocks if empty)
        Message msg = queue.pop();

        // Serialize Message object to a string
        std::string serializedMsg = msg.serialize();

        // Send the serialized message to the client
        serverSocket.async_send(asio::buffer(serializedMsg),
        [&](std::error_code ec, size_t bytesSent) {
            if (ec) {
                std::cerr << "Message not sent: " << ec.message() << "\n";
            }
        }
        );
    }
}