#include "WebsocketClient.h"
#include <iostream>

int main() {
    WebSocketClient client("127.0.0.1", "8080");
    client.connect();

    client.send("Hello from client!");
    std::string reply = client.receive();
    std::cout << "Received: " << reply << "\n";

    client.close();
}
