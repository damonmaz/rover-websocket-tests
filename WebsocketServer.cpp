#include "WebsocketServer.h"

using namespace boost;
using tcp = asio::ip::tcp;
namespace websocket = beast::websocket;

WebSocketServer::WebSocketServer(unsigned short port)
    : acceptor_(ioc_, tcp::endpoint(tcp::v4(), port)) {}

void WebSocketServer::run() {
    accept_connections();
    ioc_.run();
}

void WebSocketServer::accept_connections() {
    for (;;) {
        tcp::socket socket(ioc_);
        acceptor_.accept(socket);
        std::thread(&WebSocketServer::handle_session, this, std::move(socket)).detach();
    }
}

void WebSocketServer::handle_session(tcp::socket socket) {
    try {
        websocket::stream<tcp::socket> ws(std::move(socket));
        ws.accept();

        for (;;) {
            beast::flat_buffer buffer;
            ws.read(buffer);

            // exanoke nessage
            Message msg(1, Generic{}); 
            std::ostringstream oss;
            msg.printMessage(); 

            std::string messageToSend = "Server: Message received and processed.";

            ws.text(true);
            ws.write(asio::buffer(messageToSend));
        }
    } catch (const std::exception& e) {
        std::cerr << "Session error: " << e.what() << "\n";
    }
}