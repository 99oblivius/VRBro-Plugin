#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <asio.hpp>
#include <string>
#include <thread>
#include <memory>
#include <vector>

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

    std::string address_;
    int port_;
    std::size_t max_clients_;
    std::atomic<std::size_t> active_clients_;
    std::atomic<bool> is_running_;
    
    asio::io_context io_context_;
    asio::ip::tcp::acceptor acceptor_;
    std::thread io_thread_;
};

#endif