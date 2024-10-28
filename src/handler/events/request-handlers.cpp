#include "request-handlers.hpp"

#include <obs-frontend-api.h>

#include "plugin-support.h"

namespace RequestHandler {

void is_replay_buffer(std::string& msg, handle_error& ec) {
    (void)(ec);
    if (obs_frontend_replay_buffer_active()) {
        msg = "recording buffer";
    }
}

void is_recording(std::string& msg, handle_error& ec) {
    (void)(ec);
    if (obs_frontend_recording_active()) {
        msg = "recording";
    }
}

void is_streaming(std::string& msg, handle_error& ec) {
    (void)(ec);
    if (obs_frontend_streaming_active()) {
        msg = "streaming";
    }
}

} // namespace RequestHandler