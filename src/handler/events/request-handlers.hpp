#pragma once

#include <string>
#include <system_error>

#include <obs-frontend-api.h>

#include "../errors/handle-errors.hpp"

namespace RequestHandler {
    void is_replay_buffer(std::string& msg, handle_error& ec);
    void is_recording(std::string& msg, handle_error& ec);
    void is_streaming(std::string& msg, handle_error& ec);
    void get_current_scene(std::string& msg, handle_error& ec);
    void get_scenes(std::string& msg, handle_error& ec);
}
