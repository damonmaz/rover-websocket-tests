#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include "Message.h" 
#include <sstream>   
#include "MessageQueue.h"
#include <chrono>


class UDPServer {
public:
/** Constructor for UDPServer
     *
     * @param
     *  serverPort: unsigned short - The server port (e.g., 8080)
     *  clientPort: unsigned short - The client port (e.g., 8008)
     */
    UDPServer(unsigned short serverPort, unsigned short clientPort);

    /** Prepares the UDP server
     *
     * @param
     *  queue: MessageQueue& - The queue of messages to send to client(s)
     *
     * @return
     *  none
     */
    void run(MessageQueue& queue);

private:

    /** Continuously sends messages from the queue over UDP
     *
     * @param
     *  queue: MessageQueue& - The queue of messages to send to client(s)
     *
     * @return
     *  none
     */
    void handle_session(MessageQueue& queue);

    boost::asio::io_context ioc;   // Boost ASIO IO context
    boost::asio::ip::udp::socket serverSocket;   // UDP server socket
    boost::asio::ip::udp::endpoint clientEndpoint;
};
