#include "WebsocketServer.h"
#include "WebsocketClient.h"

#include <iostream>
#include <thread>
#include <chrono>

void server_example()
{
    std::cout << "=== WebSocket Server Example ===" << std::endl;
    
    WebSocketServer server(8080);
    
    // Set message handler
    server.set_message_handler([&server](const std::string& message) {
        std::cout << "Server received: " << message << std::endl;
        // Echo the message back to client
        server.send_message("Echo: " + message);
    });
    
    server.start();
    
    // Keep server running for demonstration
    std::this_thread::sleep_for(std::chrono::seconds(30));
    
    std::cout << "Shutting down server..." << std::endl;
    server.stop();
}

void client_example()
{
    std::cout << "=== WebSocket Client Example ===" << std::endl;
    
    WebSocketClientManager client;
    
    // Set event handlers
    client.set_connect_handler([]() {
        std::cout << "Client connected!" << std::endl;
    });
    
    client.set_disconnect_handler([]() {
        std::cout << "Client disconnected!" << std::endl;
    });
    
    client.set_message_handler([](const std::string& message) {
        std::cout << "Client received: " << message << std::endl;
    });
    
    client.start();
    
    // Connect to server
    client.connect("localhost", "8080", "/");
    
    // Wait a bit for connection
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // Send some messages
    if (client.is_connected())
    {
        client.send_message("Hello from client!");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        client.send_message("How are you, server?");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        client.send_message("Goodbye!");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    client.stop();
}

int main()
{
    try
    {
        // Run server in separate thread
        std::thread server_thread(server_example);
        
        // Give server time to start
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // Run client
        client_example();
        
        // Wait for server thread to finish
        server_thread.join();
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}