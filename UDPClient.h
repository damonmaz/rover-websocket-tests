#pragma once
#include <string>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include "Message.h" 
#include <iostream>

#define CLIENT_BUFFER_SIZE 2048 // overkill

class UDPClient {
public:

/** Constructor for UDPClient
     *
     * @param
     *  host: const std::string& - The server host address (e.g., "127.0.0.1")
     *  port: const std::string& - The server port (e.g., "8080")
     *
     * Example Usage:
     *   UDPClient client("127.0.0.1", "8080");
     */
    UDPClient(unsigned short serverPort, unsigned short clientPort);

    /** Connects to the WebSocket server
     *
     * @param
     *  none
     *
     * @return
     *  none
     */
    
    //void send(const std::string& message);

    /** Receives a message from the server
     *
     * @param
     *  none
     *
     * @return
     *  Message - The received message as a Message object
     */
    Message receive();

    /** Closes the WebSocket connection
     *
     * @param
     *  none
     *
     * @return
     *  none
     */
    //void close();

private:
    boost::asio::io_context ioc;   // Boost ASIO IO context
    boost::asio::ip::udp::socket clientSocket;   // UDP server socket
    boost::asio::ip::udp::endpoint serverEndpoint; // Endpoint includes port and IP address
};
