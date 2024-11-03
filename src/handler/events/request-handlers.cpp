#include "request-handlers.hpp"

#include <obs-frontend-api.h>

#include "plugin-support.h"
#include "../../utils.hpp"

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

void get_current_scene(std::string& msg, handle_error& ec) {
    (void)(ec);
    obs_source_t* source = obs_frontend_get_current_scene();
    if (source) {
        msg = obs_source_get_name(source);
        obs_source_release(source);
    } else {
        msg = "";
    }
}

void get_scenes(std::string& msg, handle_error& ec) {
    (void)(ec);
    char** scenes = obs_frontend_get_scene_names();
    if (scenes) {
        msg = Utils::serializeCharArrays(scenes);
        bfree(scenes);
    } else {
        msg = "";
    }
}

} // namespace RequestHandler