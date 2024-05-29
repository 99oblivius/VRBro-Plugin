#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <atomic>
#include <QString>
#include <obs-module.h>
#include <util/config-file.h>

struct Config {
    Config();
    void Load();
    void Save();
    static config_t *GetConfigStore();
    static void OBSSaveCallback(obs_data_t *config_data, bool save, void *data);

    QString ListenAddrString = "127.0.0.1";
    QString ListenPortNumber = QString::number(33390);
    std::atomic<bool> AutoBufferBool = false;
};

#endif
