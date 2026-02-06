#include "UDPReceiver.h"

int main() {
    UDPReceiver client(8080, 8008); // Localhost and port 8080

    while(true) {
        Message reply = client.receive();
        reply.printMessage(); // Print the received message
    }
}
