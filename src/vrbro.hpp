#ifndef PLUGIN_MAIN_HPP
#define PLUGIN_MAIN_HPP

#include <obs-module.h>
#include <memory>
#include "config.hpp"
#include "network/network.hpp"

struct Config;
typedef std::shared_ptr<Config> ConfigPtr;

class Network;
typedef std::shared_ptr<Network> NetworkPtr;

ConfigPtr GetConfig();
NetworkPtr GetNetwork();

void vrbro_start_server();
void vrbro_stop_server();
void vrbro_restart_server();

const char *obs_module_name();
const char *obs_module_description();
bool obs_module_load();
void obs_module_unload();

#endif
