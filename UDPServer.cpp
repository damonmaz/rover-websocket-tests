#include "UDPServer.h"


using namespace boost;
using udp = asio::ip::udp;
//namespace websocket = beast::websocket;

// Constructor
UDPServer::UDPServer(unsigned short serverPort, unsigned short clientPort) 
    : serverSocket(ioc, udp::endpoint(udp::v4(), serverPort)), 
    clientEndpoint(udp::v4(), clientPort) {}


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
        serverSocket.async_send_to(asio::buffer(serializedMsg), clientEndpoint,
        [&](std::error_code ec, size_t bytesSent) {
            if (ec) {
                std::cerr << "Message not sent: " << ec.message() << "\n";
            }
        }
        );
    }
    std::cout << "handle_session on the server side is returning!";
}