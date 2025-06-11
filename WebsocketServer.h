#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include "Message.h" 
#include <sstream>   


class WebSocketServer {
public:
/** Constructor for WebSocketServer
     *
     * @param
     *  port: unsigned short - The port number to listen for incoming connections
    */
    WebSocketServer(unsigned short port);

    /** Runs the WebSocket server
     *
     * @param
     *  msg: const Message& - The message to send to the client(s)
     *
     * @return
     *  none
     */
    void run(const Message& msg);

private:
    /** Accepts client WebSocket connections
     *
     * @param
     *  msg: const Message& - The message to send to the client(s)
     *
     * @return
     *  none
     */
    void accept_connections(const Message& msg);

    /** Handles a single WebSocket session with a connected client
     *
     * @param
     *  socket: boost::asio::ip::tcp::socket - The socket for the connected client
     *  msg: const Message& - The message to send to the client
     *
     * @return
     *  none
     */
    void handle_session(boost::asio::ip::tcp::socket socket, const Message& msg);

    boost::asio::io_context ioc;   // Boost ASIO IO context
    boost::asio::ip::tcp::acceptor acceptor;   // TCP acceptor for incoming connections
};
