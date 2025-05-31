#include "WebsocketClient.h"
#include <iostream>

// WebSocketClient Implementation
WebSocketClient::WebSocketClient(net::io_context& ioc)
    : resolver_(net::make_strand(ioc))
    , ws_(net::make_strand(ioc))
    , ioc_(ioc)
    , connected_(false)
{
}

void WebSocketClient::connect(const std::string& host, const std::string& port, const std::string& target)
{
    host_ = host;
    port_ = port;
    target_ = target;
    
    // Look up the domain name
    resolver_.async_resolve(
        host,
        port,
        beast::bind_front_handler(
            &WebSocketClient::on_resolve,
            shared_from_this()));
}

void WebSocketClient::disconnect()
{
    if (connected_)
    {
        ws_.async_close(websocket::close_code::normal,
            beast::bind_front_handler(
                &WebSocketClient::on_close,
                shared_from_this()));
    }
}

void WebSocketClient::send(const std::string& message)
{
    if (connected_)
    {
        // Post work to the strand to ensure thread safety
        net::post(ws_.get_executor(),
            [self = shared_from_this(), message]()
            {
                self->ws_.async_write(
                    net::buffer(message),
                    beast::bind_front_handler(
                        &WebSocketClient::on_write,
                        self));
            });
    }
}

bool WebSocketClient::is_connected() const
{
    return connected_.load();
}

void WebSocketClient::set_message_handler(std::function<void(const std::string&)> handler)
{
    message_handler_ = std::move(handler);
}

void WebSocketClient::set_connect_handler(std::function<void()> handler)
{
    connect_handler_ = std::move(handler);
}

void WebSocketClient::set_disconnect_handler(std::function<void()> handler)
{
    disconnect_handler_ = std::move(handler);
}

void WebSocketClient::on_resolve(beast::error_code ec, tcp::resolver::results_type results)
{
    if (ec)
    {
        std::cerr << "Resolve error: " << ec.message() << std::endl;
        return;
    }

    // Set the timeout for the operation
    beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(ws_).async_connect(
        results,
        beast::bind_front_handler(
            &WebSocketClient::on_connect,
            shared_from_this()));
}

void WebSocketClient::on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep)
{
    if (ec)
    {
        std::cerr << "Connect error: " << ec.message() << std::endl;
        return;
    }

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    beast::get_lowest_layer(ws_).expires_never();

    // Set suggested timeout settings for the websocket
    ws_.set_option(websocket::stream_base::timeout::suggested(
        beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    ws_.set_option(websocket::stream_base::decorator(
        [](websocket::request_type& req)
        {
            req.set(http::field::user_agent, "WebSocket-Client");
        }));

    // Update the host string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    host_ += ':' + std::to_string(ep.port());

    // Perform the websocket handshake
    ws_.async_handshake(host_, target_,
        beast::bind_front_handler(
            &WebSocketClient::on_handshake,
            shared_from_this()));
}

void WebSocketClient::on_handshake(beast::error_code ec)
{
    if (ec)
    {
        std::cerr << "Handshake error: " << ec.message() << std::endl;
        return;
    }

    connected_ = true;
    std::cout << "Connected to WebSocket server" << std::endl;
    
    if (connect_handler_)
        connect_handler_();

    // Start reading messages
    do_read();
}

void WebSocketClient::do_read()
{
    // Read a message into our buffer
    ws_.async_read(
        buffer_,
        beast::bind_front_handler(
            &WebSocketClient::on_read,
            shared_from_this()));
}

void WebSocketClient::on_read(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec)
    {
        if (ec == websocket::error::closed)
        {
            connected_ = false;
            std::cout << "Connection closed by server" << std::endl;
            if (disconnect_handler_)
                disconnect_handler_();
        }
        else
        {
            std::cerr << "Read error: " << ec.message() << std::endl;
        }
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

void WebSocketClient::on_write(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec)
    {
        std::cerr << "Write error: " << ec.message() << std::endl;
        return;
    }
}

void WebSocketClient::on_close(beast::error_code ec)
{
    connected_ = false;
    
    if (ec)
    {
        std::cerr << "Close error: " << ec.message() << std::endl;
    }
    else
    {
        std::cout << "Disconnected from WebSocket server" << std::endl;
    }
    
    if (disconnect_handler_)
        disconnect_handler_();
}

// WebSocketClientManager Implementation
WebSocketClientManager::WebSocketClientManager()
    : running_(false)
{
}

WebSocketClientManager::~WebSocketClientManager()
{
    stop();
}

void WebSocketClientManager::start()
{
    if (running_)
        return;

    running_ = true;
    client_ = std::make_shared<WebSocketClient>(ioc_);
    
    client_thread_ = std::thread([this]() {
        ioc_.run();
    });
}

void WebSocketClientManager::stop()
{
    if (!running_)
        return;

    running_ = false;
    
    if (client_)
    {
        client_->disconnect();
    }
    
    ioc_.stop();
    
    if (client_thread_.joinable())
    {
        client_thread_.join();
    }
    
    // Reset the io_context for potential reuse
    ioc_.reset();
}

void WebSocketClientManager::connect(const std::string& host, const std::string& port, const std::string& target)
{
    if (client_)
    {
        client_->connect(host, port, target);
    }
}

void WebSocketClientManager::disconnect()
{
    if (client_)
    {
        client_->disconnect();
    }
}

void WebSocketClientManager::send_message(const std::string& message)
{
    if (client_)
    {
        client_->send(message);
    }
}

bool WebSocketClientManager::is_connected() const
{
    return client_ && client_->is_connected();
}

void WebSocketClientManager::set_message_handler(std::function<void(const std::string&)> handler)
{
    if (client_)
    {
        client_->set_message_handler(std::move(handler));
    }
}

void WebSocketClientManager::set_connect_handler(std::function<void()> handler)
{
    if (client_)
    {
        client_->set_connect_handler(std::move(handler));
    }
}

void WebSocketClientManager::set_disconnect_handler(std::function<void()> handler)
{
    if (client_)
    {
        client_->set_disconnect_handler(std::move(handler));
    }
}