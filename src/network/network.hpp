#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <asio.hpp>

class Network {
public:
    Network(const std::string& address, int port, std::size_t max_clients);
    ~Network();

    void start();
    void shutdown();

private:
    void accept_connections();
    void handle_accept(const asio::error_code& error, std::shared_ptr<asio::ip::tcp::socket> client_socket);
    void read_message(std::shared_ptr<asio::ip::tcp::socket> client_socket);
    void send_message(std::shared_ptr<asio::ip::tcp::socket> client_socket, const std::vector<uint8_t>& packet);

    // Network configuration
    std::string address_;
    int port_;
    std::size_t max_clients_;
    std::atomic<std::size_t> active_clients_;
    std::atomic<bool> is_running_;
    
    // ASIO components
    asio::io_context io_context_;
    asio::ip::tcp::acceptor acceptor_{io_context_};
    std::thread io_thread_;
};
