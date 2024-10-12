#include <iostream>
#include <obs-module.h>

#include "plugin-support.h"
#include "network.hpp"
#include "../handler/handler.hpp"

using json = nlohmann::json;

Network::Network(const std::string& address, int port, std::size_t max_clients) : 
    address_(address), port_(port), max_clients_(max_clients), active_clients_(0),
    is_running_(false), acceptor_(io_context_, asio::ip::tcp::endpoint(asio::ip::make_address(address), port)) {}

Network::~Network() {
    shutdown();
}

void Network::start() {
    is_running_ = true;
    accept_connections();

    io_thread_ = std::thread([this]() {
        io_context_.run();
    });
}

void Network::shutdown() {
    is_running_ = false;
    io_context_.stop();
    if (io_thread_.joinable()) {
        io_thread_.join();
    }
}

void Network::accept_connections() {
    auto client_socket = std::make_shared<asio::ip::tcp::socket>(io_context_);
    acceptor_.async_accept(*client_socket, [this, client_socket](const asio::error_code& error) {
        handle_accept(error, client_socket);
    });
}

void Network::handle_accept(const asio::error_code& error, std::shared_ptr<asio::ip::tcp::socket> client_socket) {
    if (!error) {
        active_clients_++;
        obs_log(LOG_INFO, "[Network::handle_accept] %d Connected new socket", error.value());
        read_message(client_socket);
    }
    if (is_running_) {
        accept_connections();
    }
}
void Network::read_message(std::shared_ptr<asio::ip::tcp::socket> client_socket) {
    auto buf = std::make_shared<asio::streambuf>();
    asio::async_read_until(*client_socket, *buf, "\n", [this, client_socket, buf](const asio::error_code& error, std::size_t) {
        std::string_view packet(asio::buffer_cast<const char*>(buf->data()), buf->size());
        obs_log(LOG_INFO, "[message received]: %s", packet.c_str());
        if (!error) {
            std::vector<uint8_t> response;
            std::error_code ec;
            CommandHandler::Handler(packet, response, ec);

            if (ec) obs_log(LOG_INFO, "[Network::read_message] error: %s", ec.message().c_str());
            send_message(client_socket, response);
            if (is_running_) read_message(client_socket);
        } else {
            obs_log(LOG_INFO, "[Network::read_message] Closing socket: %s", error.message().c_str());
            client_socket->close();
            active_clients_--;
        }
    });
}

void Network::send_message(std::shared_ptr<asio::ip::tcp::socket> client_socket, const std::vector<uint8_t>& packet) {
    auto message_buf = std::make_shared<std::string>(packet.begin(), packet.end());
    asio::async_write(*client_socket, asio::buffer(*message_buf), [message_buf, this, client_socket](const asio::error_code& error, std::size_t) {
        if (error) {
            obs_log(LOG_INFO, "[Network::send_message] Error sending message: %s", error.message().c_str());
            client_socket->close();
            active_clients_--;
        }
    });
}
