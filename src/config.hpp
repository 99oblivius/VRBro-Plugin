#pragma once

#include <atomic>

#include <QString>
#include <obs-module.h>
#include <util/config-file.h>

// Configuration constants
namespace ConfigConstants {
    constexpr const char* DEFAULT_ADDRESS = "127.0.0.1";
    constexpr int DEFAULT_PORT = 33390;
}

class Config {
public:
    Config();

    void Load();
    void Save();
    static config_t* GetConfigStore();
    static void OBSSaveCallback(obs_data_t* config_data, bool save, void* data);

    QString ListenAddrString{ConfigConstants::DEFAULT_ADDRESS};
    QString ListenPortNumber{QString::number(ConfigConstants::DEFAULT_PORT)};
    std::atomic<bool> AutoBufferBool{false};
};
