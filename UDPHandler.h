#pragma once
#include <boost/asio.hpp>
#include "Message.h" 
#include <iostream>
#include "Message.h" 
#include "MessageQueue.h"
#include "RoverState/MotorState.h"
#include "RoverState/StateManagers/MotorStateManager.h"


#define RECEIVE_BUFFER_SIZE 100

class UDPHandler {
public:

/** Constructor for UDPReceiver
     *
     * @param
     *  myPort: unsigned short - The port the current instantiation should use (e.g., 8080)
     *  theirPort: unsigned short - The port to communicate to (e.g., 8008)
     *
     * Example Usage:
     *   UDPReceiver client(8080, 8008);
     */
    UDPHandler(unsigned short myPort, unsigned short theirPort);

    /** Prepares the UDP handler
     *
     * @param
     *  queue: MessageQueue& - The queue of messages to send
     *
     * @return
     *  none
     */
    void run(MessageQueue& queue);

    /** Continuously sends messages from the queue over UDP
     *
     * @param
     *  queue: MessageQueue& - The queue of messages to send
     *
     * @return
     *  none
     */
    void handle_session(MessageQueue& queue);

    /** Receives a message from the server
     *
     * @param
     *  none
     *
     * @return
     *  Message - The received message as a Message object
     */
    void receive(MessageQueue& queue);

private:
    boost::asio::io_context ioc;   // Boost ASIO IO context

    boost::asio::ip::udp::socket mySocket;   
    boost::asio::ip::udp::endpoint theirEndpoint; // Endpoint includes port and IP address

    std::vector<std::byte> received;
    boost::system::error_code ec;

    bool isReceiving;
};
