#pragma once

#include <memory>

#include <obs-module.h>

// Forward declarations
class Config;
class Network;

using ConfigPtr = std::shared_ptr<Config>;
using NetworkPtr = std::shared_ptr<Network>;

// Global accessors
ConfigPtr GetConfig();
NetworkPtr GetNetwork();

// Server control functions
void vrbro_start_server();
void vrbro_stop_server();
void vrbro_restart_server();

// OBS module interface
const char* obs_module_name();
const char* obs_module_description();
bool obs_module_load();
void obs_module_unload();