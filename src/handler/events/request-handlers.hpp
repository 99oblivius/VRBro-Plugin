#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <string>
#include <obs-frontend-api.h>
#include <nlohmann/json.hpp>

#include "../errors/handle-errors.hpp"
#include "plugin-support.h"

using json = nlohmann::json;

namespace RequestHandler {
    void is_replay_buffer(std::string& msg, handle_error& ec);
    void is_recording(std::string& msg, handle_error& ec);
    void is_streaming(std::string& msg, handle_error& ec);
}

#endif
