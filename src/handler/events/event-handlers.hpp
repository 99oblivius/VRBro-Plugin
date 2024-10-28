#pragma once

#include <string>
#include <system_error>

#include <obs-frontend-api.h>

#include "../errors/handle-errors.hpp"

namespace EventHandler {
    void start_replay_buffer(std::string& msg, handle_error& ec);
    void stop_replay_buffer(std::string& msg, handle_error& ec);
    void save_replay_buffer(std::string& msg, handle_error& ec);
    void start_recording(std::string& msg, handle_error& ec);
    void stop_recording(std::string& msg, handle_error& ec);
    void start_streaming(std::string& msg, handle_error& ec);
    void stop_streaming(std::string& msg, handle_error& ec);
    void recording_split_file(std::string& msg, handle_error& ec);
}
