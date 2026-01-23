#include "UDPServer.h"

int main() {
    MessageQueue queue;

    // Push messages into the queue
    queue.push(Message(0, Generic{77}));
    queue.push(Message(1, WheelMessage{120, 45, 10}));
    queue.push(Message(1, ArmMessage{100, 200, 300, 50, 60, 1, 90, 180}));

    UDPServer server(8080);
    server.run(queue);
}
