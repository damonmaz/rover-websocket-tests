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
/** Constructor for WebSocketServer
     *
     * @param
     *  port: unsigned short - The port number to listen for incoming connections
    */
    UDPServer(unsigned short serverPort, unsigned short clientPort);

    /** Runs the WebSocket server
     *
     * @param
     *  msg: const Message& - The message to send to the client(s)
     *
     * @return
     *  none
     */
    void run(MessageQueue& queue);

private:

    /** Handles a single WebSocket session with a connected client
     *
     * @param
     *  socket: boost::asio::ip::tcp::socket - The socket for the connected client
     *  msg: const Message& - The message to send to the client
     *
     * @return
     *  none
     */
    void handle_session(MessageQueue& queue);

    boost::asio::io_context ioc;   // Boost ASIO IO context
    boost::asio::ip::udp::socket serverSocket;   // UDP server socket
    boost::asio::ip::udp::endpoint clientEndpoint;
};
