#include "event-handlers.hpp"

#include <obs-frontend-api.h>

#include "plugin-support.h"

namespace EventHandler {

void start_replay_buffer(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_replay_buffer_active()) {
        ec = make_error_code(HandleError::ReplayBufferError);
    } else {
        obs_frontend_replay_buffer_start();
    }
}

void stop_replay_buffer(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_replay_buffer_active()) {
        obs_frontend_replay_buffer_stop();
    } else {
        ec = make_error_code(HandleError::ReplayBufferError);
    }
}

void save_replay_buffer(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_replay_buffer_active()) {
        obs_frontend_replay_buffer_save();
    } else {
        ec = make_error_code(HandleError::ReplayBufferError);
    }
}

void start_recording(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_recording_active()) {
        ec = make_error_code(HandleError::RecordingError);
    } else {
        obs_frontend_recording_start();
    }
}

void stop_recording(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_recording_active()) {
        obs_frontend_recording_stop();
    } else {
        ec = make_error_code(HandleError::RecordingError);
    }
}

void start_streaming(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_streaming_active()) {
        ec = make_error_code(HandleError::StreamingError);
    } else {
        obs_frontend_streaming_start();
    }
}

void stop_streaming(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_streaming_active()) {
        obs_frontend_streaming_stop();
    } else {
        ec = make_error_code(HandleError::StreamingError);
    }
}

void recording_split_file(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_recording_active()) {
        obs_frontend_recording_split_file();
    } else {
        ec = make_error_code(HandleError::RecordingError);
    }
}

void set_scene(std::string& payload, handle_error& ec) {
    if (payload.empty()) {
        ec = make_error_code(HandleError::SceneError); 
        return;
    }

    obs_source_t* scene = obs_get_source_by_name(payload.c_str());
    if (!scene) {
        ec = make_error_code(HandleError::SceneError);
        return;
    }

    obs_frontend_set_current_scene(scene);
    obs_source_release(scene);
    payload.clear();
}

} // namespace EventHandler