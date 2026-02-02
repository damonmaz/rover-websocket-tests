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
    
    // Create some fake asio tasks - prevent the context from finishing
    asio::io_context::work idleWork(ioc);
    ioc.run();
}

// Handle a single WebSocket session with a connected client
void UDPServer::handle_session(MessageQueue& queue) {
    while(true) {
        // Pop the next message from the queue (blocks if empty)
        Message msg = queue.pop();

        // Serialize Message object to a string
        std::vector<std::byte> serializedMsg = msg.serialize();
        asio::mutable_buffer msgBuffer = asio::buffer(serializedMsg);
        std::cout << "Size prepared for sending: " << msgBuffer.size() << "\n";

        //serverSocket.wait(serverSocket.wait_write); //Wait until write is allowed

        // Send the serialized message to the client
        size_t returned = serverSocket.send_to(msgBuffer, clientEndpoint);
        std::cout << "Size actually sent: " << returned << "\n";
    }
    //serverSocket.send_to(asio::buffer("Hello from Server."), clientEndpoint);
}