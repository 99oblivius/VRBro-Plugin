#include "vrbro.hpp"

#include <QAction>
#include <QMainWindow>
#include <QMessageBox>

#include <obs-frontend-api.h>
#include <obs-module.h>
#include <string>

#include "config.hpp"
#include "forms/server-settings.hpp"
#include "network/network.hpp"
#include "plugin-support.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

namespace {
    ConfigPtr _conf;
    NetworkPtr _net;
    ServerSettings* server_settings = nullptr;
}

ConfigPtr GetConfig() { return _conf; }
NetworkPtr GetNetwork() { return _net; }

const char* obs_module_name() {
    return "VRBro Server";
}

const char* obs_module_description() {
    return "VRBro server plugin for OBS Studio";
}

void vrbro_start_server() {
    obs_log(LOG_INFO, "[vrbro-server] Server Start!");
    _net->start();
}

void vrbro_stop_server() {
    obs_log(LOG_INFO, "[vrbro-server] Server Halt!");
    _net->shutdown();
}

void vrbro_restart_server() {
    vrbro_stop_server();
    _net = std::make_shared<Network>(
        _conf->ListenAddrString.toStdString(),
        _conf->ListenPortNumber.toInt(),
        3
    );
    vrbro_start_server();
}

bool obs_module_load(void) {
    QMainWindow* main_window = static_cast<QMainWindow*>(obs_frontend_get_main_window());
    if (!main_window) return true;

    _conf = std::make_shared<Config>();
    _conf->Load();

    // Try configured address, fallback to default if needed
    obs_log(LOG_WARNING, "[vrbro-server] Attempting network init: %s:%d", 
        _conf->ListenAddrString.toUtf8().constData(),
        _conf->ListenPortNumber.toInt());

    try {
        _net = std::make_shared<Network>(
            _conf->ListenAddrString.toStdString(),
            _conf->ListenPortNumber.toInt(), 3);
    } catch (const std::exception& e) {
        obs_log(LOG_ERROR, "[vrbro-server] Network init failed: %s - Trying default", e.what());
        try {
            _net = std::make_shared<Network>(ConfigConstants::DEFAULT_ADDRESS,
                _conf->ListenPortNumber.toInt(), 3);
            _conf->ListenAddrString = ConfigConstants::DEFAULT_ADDRESS;
        } catch (const std::exception& e) {
            obs_log(LOG_ERROR, "[vrbro-server] Default address also failed: %s", e.what());
        }
    }

    auto menu_action = static_cast<QAction*>(
        obs_frontend_add_tools_menu_qaction(obs_module_text("VRBro Server Settings")));

    obs_frontend_push_ui_translation(obs_module_get_string);
    server_settings = new ServerSettings(main_window);
    obs_frontend_pop_ui_translation();

    menu_action->connect(menu_action, &QAction::triggered, 
        [] { server_settings->ToggleShowHide(); });

    obs_frontend_add_event_callback(
        [](enum obs_frontend_event event, void*) {
            if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING) {
                config_t* obs_conf = obs_frontend_get_global_config();
                if (config_get_bool(obs_conf, "VRBroServerPlugin", "AutoBufferBool") && 
                    !obs_frontend_replay_buffer_active()) {
                    obs_frontend_replay_buffer_start();
                }
                vrbro_start_server();
            } else if (event == OBS_FRONTEND_EVENT_EXIT) {
                vrbro_stop_server();
            }
        }, 
        static_cast<void*>(&_conf)
    );

    obs_log(LOG_INFO, "[vrbro-server] VRBro Server version: %s", PLUGIN_VERSION);
    return true;
}

void obs_module_unload(void) {
    obs_log(LOG_INFO, "[vrbro-server] VRBro Server unloaded");
    vrbro_stop_server();
}