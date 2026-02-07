#include "UDPHandler.h"

int main() {
    MessageQueue queue;

    // Push messages into the queue
    queue.push(Message(Generic{76}));
    queue.push(Message(WheelMessage{120, 45, 10}));
    queue.push(Message(ArmMessage{100, 200, 300, 50, 60, 1, 90, 180}));

    UDPHandler client(8008, 8080);
    client.run(queue);
}