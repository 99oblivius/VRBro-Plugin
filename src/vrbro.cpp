#include "vrbro.hpp"
#include "config.hpp"
#include "network/network.hpp"
#include "plugin-support.h"
#include "forms/server-settings.hpp"

#include <obs-frontend-api.h>
#include <obs-module.h>
#include <QAction>
#include <QMainWindow>
#include <QMessageBox>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

ConfigPtr _conf;
NetworkPtr _net;

ConfigPtr GetConfig() { return _conf; }
NetworkPtr GetNetwork() { return _net; }

ServerSettings *server_settings = nullptr;

const char* obs_module_name() {
    return "VRBro Server";
}

const char* obs_module_description() {
    return "VRBro server plugin for OBS Studio";
}

void vrbro_start_server() {
    QMainWindow* main_window = (QMainWindow*)obs_frontend_get_main_window();
    obs_log(LOG_INFO, "[vrbro-server] Server Start!");
    _net->start();
}

void vrbro_stop_server() {
    obs_log(LOG_INFO, "[vrbro-server] Server Halt!");
    _net->shutdown();
}

void vrbro_restart_server() {
    vrbro_stop_server();
    _net = std::make_shared<Network>(_conf->ListenAddrString.toStdString(), _conf->ListenPortNumber.toInt(), 3);
    vrbro_start_server();
}

bool obs_module_load(void) {
    QMainWindow* main_window = (QMainWindow*)obs_frontend_get_main_window();

    if (main_window) {
        _conf = std::make_shared<Config>();
        _conf->Load();

        _net = std::make_shared<Network>(_conf->ListenAddrString.toStdString(), _conf->ListenPortNumber.toInt(), 3);

        QAction* menu_action = (QAction*)obs_frontend_add_tools_menu_qaction(obs_module_text("VRBro Server Settings"));

        obs_frontend_push_ui_translation(obs_module_get_string);
        server_settings = new ServerSettings(main_window);
        obs_frontend_pop_ui_translation();

        auto menu_cb = [] {
            server_settings->ToggleShowHide();
        };
        menu_action->connect(menu_action, &QAction::triggered, menu_cb);

        obs_frontend_add_event_callback(
            [](enum obs_frontend_event event, void* private_data) {
                if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING) {
                    config_t* obs_conf = obs_frontend_get_global_config();
                    if (config_get_bool(obs_conf, "VRBroServerPlugin", "AutoBufferBool") && !obs_frontend_replay_buffer_active()) {
                        obs_frontend_replay_buffer_start();
                    }
                    vrbro_start_server();
                } else if (event == OBS_FRONTEND_EVENT_EXIT) {
                    vrbro_stop_server();
                }
            }, static_cast<void*>(&_conf));
    }

    obs_log(LOG_INFO, "[vrbro-server] VRBro Server version: %s", PLUGIN_VERSION);
    return true;
}

void obs_module_unload(void) {
    obs_log(LOG_INFO, "[vrbro-server] VRBro Server unloaded");
    vrbro_stop_server();
}
