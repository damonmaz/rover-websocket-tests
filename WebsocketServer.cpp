#include "WebsocketServer.h"
#include <iostream>

// WebSocketSession Implementation
WebSocketSession::WebSocketSession(tcp::socket&& socket)
    : ws_(std::move(socket))
{
}

void WebSocketSession::set_message_handler(std::function<void(const std::string&)> handler)
{
    message_handler_ = std::move(handler);
}

void WebSocketSession::run()
{
    // Set suggested timeout settings for the websocket
    ws_.set_option(websocket::stream_base::timeout::suggested(
        beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    ws_.set_option(websocket::stream_base::decorator(
        [](websocket::response_type& res)
        {
            res.set(http::field::server, "WebSocket-Server");
        }));

    // Accept the websocket handshake
    ws_.async_accept(
        beast::bind_front_handler(
            &WebSocketSession::on_accept,
            shared_from_this()));
}

void WebSocketSession::send(const std::string& message)
{
    // Post work to the strand to ensure thread safety
    net::post(ws_.get_executor(),
        [self = shared_from_this(), message]()
        {
            self->ws_.async_write(
                net::buffer(message),
                beast::bind_front_handler(
                    &WebSocketSession::on_write,
                    self));
        });
}

void WebSocketSession::close()
{
    ws_.async_close(websocket::close_code::normal,
        [](beast::error_code ec) {
            if (ec)
                std::cerr << "Close error: " << ec.message() << std::endl;
        });
}

void WebSocketSession::on_accept(beast::error_code ec)
{
    if (ec)
    {
        std::cerr << "Accept error: " << ec.message() << std::endl;
        return;
    }

    // Read a message
    do_read();
}

void WebSocketSession::do_read()
{
    // Read a message into our buffer
    ws_.async_read(
        buffer_,
        beast::bind_front_handler(
            &WebSocketSession::on_read,
            shared_from_this()));
}

void WebSocketSession::on_read(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This indicates that the session was closed
    if (ec == websocket::error::closed)
        return;

    if (ec)
    {
        std::cerr << "Read error: " << ec.message() << std::endl;
        return;
    }

    // Handle the message
    if (message_handler_)
    {
        std::string message = beast::buffers_to_string(buffer_.data());
        message_handler_(message);
    }

    // Clear the buffer
    buffer_.consume(buffer_.size());

    // Read another message
    do_read();
}

void WebSocketSession::on_write(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec)
    {
        std::cerr << "Write error: " << ec.message() << std::endl;
        return;
    }
}

// WebSocketServer Implementation
WebSocketServer::WebSocketServer(unsigned short port)
    : acceptor_(ioc_, {tcp::v4(), port})
    , running_(false)
{
}

WebSocketServer::~WebSocketServer()
{
    stop();
}

void WebSocketServer::start()
{
    if (running_)
        return;

    running_ = true;
    do_accept();
    
    server_thread_ = std::thread([this]() {
        ioc_.run();
    });
    
    std::cout << "WebSocket server started on port " << acceptor_.local_endpoint().port() << std::endl;
}

void WebSocketServer::stop()
{
    if (!running_)
        return;

    running_ = false;
    
    if (current_session_)
    {
        current_session_->close();
    }
    
    ioc_.stop();
    
    if (server_thread_.joinable())
    {
        server_thread_.join();
    }
    
    std::cout << "WebSocket server stopped" << std::endl;
}

void WebSocketServer::set_message_handler(std::function<void(const std::string&)> handler)
{
    message_handler_ = std::move(handler);
}

void WebSocketServer::send_message(const std::string& message)
{
    if (current_session_)
    {
        current_session_->send(message);
    }
}

void WebSocketServer::do_accept()
{
    acceptor_.async_accept(
        net::make_strand(ioc_),
        beast::bind_front_handler(
            &WebSocketServer::on_accept,
            this));
}

void WebSocketServer::on_accept(beast::error_code ec, tcp::socket socket)
{
    if (ec)
    {
        std::cerr << "Accept error: " << ec.message() << std::endl;
    }
    else
    {
        // Create the session and run it
        current_session_ = std::make_shared<WebSocketSession>(std::move(socket));
        current_session_->set_message_handler(message_handler_);
        current_session_->run();
        
        std::cout << "Client connected" << std::endl;
    }

    // Accept another connection if still running
    if (running_)
    {
        do_accept();
    }
}