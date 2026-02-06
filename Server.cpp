#include "UDPSender.h"

int main() {
    MessageQueue queue;

    // Push messages into the queue
    queue.push(Message(Generic{76}));
    //queue.push(Message(0, Generic{77}));
    //queue.push(Message(0, Generic{78}));
    queue.push(Message(WheelMessage{120, 45, 10}));
    queue.push(Message(ArmMessage{100, 200, 300, 50, 60, 1, 90, 180}));

    //TEMP
    std::cout << "Message size:";
    std::vector<std::byte> serializedMsg = Message(Generic{76}).serialize();
    boost::asio::mutable_buffer msgBuffer = boost::asio::buffer(serializedMsg);
    std::cout << msgBuffer.size() << "\n";

    UDPSender server(8080, 8008);
    server.run(queue);
    std::cout << "Data sent \n";
}
