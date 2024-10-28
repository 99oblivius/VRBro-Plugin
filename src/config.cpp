#include <obs-frontend-api.h>
#include <util/config-file.h>
#include <QString>
#include <QMessageBox>
#include <QRegularExpression>
#include <obs-module.h>
#include <plugin-support.h>
#include <fstream>

#include "config.hpp"
#include <nlohmann/json.hpp>
#include "vrbro.hpp"

using json = nlohmann::json;

#define TITLE "VRBroServerPlugin"
#define LISTEN_ADDR_STRING "ListenAddrString"
#define LISTEN_PORT_NUMBER "ListenPortNumber"
#define AUTO_BUFFER "AutoBufferBool"

bool isValidAddr(const QString &str_ip) {
    QRegularExpression addrRegex(R"(^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)");
    QRegularExpressionMatch match = addrRegex.match(str_ip);
    return match.hasMatch();
}

bool isValidRange(const QString &str_port) {
    bool ok;
    qulonglong value = str_port.toULongLong(&ok);
    return ok && value >= 1024 && value <= 65353;
}


Config::Config() {
    config_t *obs_config = GetConfigStore();
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
    config_t *obs_config = GetConfigStore();
    if (obs_config) {
        ListenAddrString = config_get_string(obs_config, TITLE, LISTEN_ADDR_STRING);
        ListenPortNumber = QString::number(config_get_uint(obs_config, TITLE, LISTEN_PORT_NUMBER));
        AutoBufferBool = config_get_bool(obs_config, TITLE, AUTO_BUFFER);
    }
}

void Config::Save() {
    config_t *obs_config = GetConfigStore();
    if (obs_config) {

        ListenAddrString.remove(QRegularExpression("\\s+"));
        if (!isValidAddr(ListenAddrString)) {
            QMessageBox alertbox;
            alertbox.setWindowTitle("VRBro.Address.Error");
            alertbox.setText("x.x.x.x\n\nChoose a valid listening address\ne.g 0.0.0.0");
            alertbox.exec();
            return;
        }
        config_set_string(obs_config, TITLE, LISTEN_ADDR_STRING, ListenAddrString.toUtf8().constData());
        
        if (!isValidRange(ListenPortNumber)) {
            QMessageBox alertbox;
            alertbox.setWindowTitle("VRBro.Port.Error");
            alertbox.setText("1024 <-> 65535]\n\nChoose a valid port number\ne.g 33390");
            alertbox.exec();
            return;
        }
        config_set_uint(obs_config, TITLE, LISTEN_PORT_NUMBER, ListenPortNumber.toULongLong());

        config_set_bool(obs_config, TITLE, AUTO_BUFFER, AutoBufferBool);
        
        obs_log(LOG_WARNING, "[VRBro::Config] listen: %s  port: %d", ListenAddrString.toUtf8().constData(), ListenPortNumber.toULongLong());
        config_save(obs_config);

        vrbro_restart_server();
    }
}

config_t *Config::GetConfigStore() {
	return obs_frontend_get_global_config();
}
