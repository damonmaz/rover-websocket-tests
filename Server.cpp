#include "UDPServer.h"

int main() {
    MessageQueue queue;

    // Push messages into the queue
    queue.push(Message(0, Generic{76}));
    //queue.push(Message(0, Generic{77}));
    //queue.push(Message(0, Generic{78}));
    queue.push(Message(1, WheelMessage{120, 45, 10}));
    queue.push(Message(1, ArmMessage{100, 200, 300, 50, 60, 1, 90, 180}));

    //TEMP
    std::cout << "Message size:";
    std::string serializedMsg = Message(0, Generic{76}).serialize();
    boost::asio::mutable_buffer msgBuffer = boost::asio::buffer(serializedMsg);
    std::cout << msgBuffer.size() << "\n";

    UDPServer server(8080, 8008);
    server.run(queue);
    std::cout << "Data sent \n";
}
