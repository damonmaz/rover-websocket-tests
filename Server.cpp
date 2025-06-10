#include "WebsocketServer.h"

int main() {
    WebSocketServer server(8080);
    server.run();
}
