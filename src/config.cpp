#include "config.hpp"

#include <fstream>

#include <QMessageBox>
#include <QRegularExpression>
#include <QString>
#include <obs-frontend-api.h>
#include <obs-module.h>
#include <util/config-file.h>
#include <asio.hpp>

#include "plugin-support.h"
#include "vrbro.hpp"

namespace {
    constexpr const char* TITLE = "VRBroServerPlugin";
    constexpr const char* LISTEN_ADDR_STRING = "ListenAddrString";
    constexpr const char* LISTEN_PORT_NUMBER = "ListenPortNumber";
    constexpr const char* AUTO_BUFFER = "AutoBufferBool";

    bool isValidAddr(const QString& str_ip) {
        static const QRegularExpression addrRegex(
            R"(^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)"
        );
        return addrRegex.match(str_ip).hasMatch();
    }

    bool isValidPort(const QString& str_port) {
        bool ok;
        qulonglong value = str_port.toULongLong(&ok);
        return ok && value >= 1024 && value <= 65535;
    }
}

Config::Config() {
    config_t* obs_config = GetConfigStore();
    if (obs_config) {
        config_set_default_string(obs_config, TITLE, LISTEN_ADDR_STRING, 
            ListenAddrString.toUtf8().constData());
        config_set_default_uint(obs_config, TITLE, LISTEN_PORT_NUMBER, 
            ListenPortNumber.toULongLong());
        config_set_default_bool(obs_config, TITLE, AUTO_BUFFER, 
            AutoBufferBool);
    }
}

void Config::Load() {
    config_t* obs_config = GetConfigStore();
    if (obs_config) {
        ListenAddrString = QString::fromUtf8(config_get_string(obs_config, TITLE, LISTEN_ADDR_STRING));
        ListenPortNumber = QString::number(config_get_uint(obs_config, TITLE, LISTEN_PORT_NUMBER));
        AutoBufferBool = config_get_bool(obs_config, TITLE, AUTO_BUFFER);
    }
}

void Config::Save() {
    config_t* obs_config = GetConfigStore();
    if (!obs_config) return;

    // Skip test if already using default address
    ListenAddrString.remove(QRegularExpression("\\s+"));
    if (ListenAddrString != ConfigConstants::DEFAULT_ADDRESS) {
        try {
            asio::io_context test_context;
            asio::ip::tcp::acceptor test_acceptor(test_context);
            auto endpoint = asio::ip::tcp::endpoint(
                asio::ip::make_address(ListenAddrString.toStdString()),
                33390
            );
            test_acceptor.open(endpoint.protocol());
            test_acceptor.bind(endpoint);
            test_acceptor.close();
            test_context.stop();
        } catch (...) {
            QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "VRBro.Address.Error",
                "Address cannot be used. Use default (127.0.0.1)?",
                QMessageBox::Yes | QMessageBox::No);
                
            if (reply == QMessageBox::Yes) {
                ListenAddrString = ConfigConstants::DEFAULT_ADDRESS;
            } else {
                return;
            }
        }
    }

    // Port validation
    bool ok;
    uint32_t port = ListenPortNumber.toUInt(&ok);
    if (!ok || port < 1024 || port > 65535) {
        QMessageBox::warning(nullptr, "VRBro.Port.Error", "Invalid port (use 1024-65535)");
        return;
    }

    // Save settings
    config_set_string(obs_config, TITLE, LISTEN_ADDR_STRING, ListenAddrString.toUtf8().constData());
    config_set_uint(obs_config, TITLE, LISTEN_PORT_NUMBER, port);
    config_set_bool(obs_config, TITLE, AUTO_BUFFER, AutoBufferBool);
    
    config_save(obs_config);
    vrbro_restart_server();
}

config_t* Config::GetConfigStore() {
    return obs_frontend_get_global_config();
}