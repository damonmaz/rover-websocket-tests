#pragma once
#include <string>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include "Message.h" 
#include <iostream>

#define CLIENT_BUFFER_SIZE 100 // overkill

class UDPClient {
public:

/** Constructor for UDPClient
     *
     * @param
     *  serverPort: unsigned short - The server port (e.g., 8080)
     *  clientPort: unsigned short - The client port (e.g., 8008)
     *
     * Example Usage:
     *   UDPClient client(8080, 8008);
     */
    UDPClient(unsigned short serverPort, unsigned short clientPort);

    /** Receives a message from the server
     *
     * @param
     *  none
     *
     * @return
     *  Message - The received message as a Message object
     */
    Message receive();

private:
    boost::asio::io_context ioc;   // Boost ASIO IO context
    boost::asio::ip::udp::socket clientSocket;   // UDP client socket
    boost::asio::ip::udp::endpoint serverEndpoint; // Endpoint includes port and IP address
};
