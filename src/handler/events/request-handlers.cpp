#include "request-handlers.hpp"

void RequestHandler::is_replay_buffer(std::string& msg, handle_error& ec) {
    (void)(ec);
    if (obs_frontend_replay_buffer_active()) {
        msg = "recording buffer";
    }
}

void RequestHandler::is_recording(std::string& msg, handle_error& ec) {
    (void)(ec);
    if (obs_frontend_recording_active()) {
        msg = "recording";
    }
}

void RequestHandler::is_streaming(std::string& msg, handle_error& ec) {
    (void)(ec);
    if (obs_frontend_streaming_active()) {
        msg = "streaming";
    }
}
