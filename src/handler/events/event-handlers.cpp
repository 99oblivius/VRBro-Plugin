#include "event-handlers.hpp"

void EventHandler::start_replay_buffer(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_replay_buffer_active()) {
        ec = make_error_code(HandleError::ReplayBufferError);
    } else {
        obs_frontend_replay_buffer_start();
    }
}

void EventHandler::stop_replay_buffer(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_replay_buffer_active()) {
        obs_frontend_replay_buffer_stop();
    } else {
        ec = make_error_code(HandleError::ReplayBufferError);
    }
}

void EventHandler::save_replay_buffer(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_replay_buffer_active()) {
        obs_frontend_replay_buffer_save();
    } else {
        ec = make_error_code(HandleError::ReplayBufferError);
    }
}

void EventHandler::start_recording(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_recording_active()) {
        ec = make_error_code(HandleError::RecordingError);
    } else {
        obs_frontend_recording_start();
    }
}

void EventHandler::stop_recording(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_recording_active()) {
        obs_frontend_recording_stop();
    } else {
        ec = make_error_code(HandleError::RecordingError);
    }
}

void EventHandler::start_streaming(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_streaming_active()) {
        ec = make_error_code(HandleError::StreamingError);
    } else {
        obs_frontend_streaming_start();
    }
}

void EventHandler::stop_streaming(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_streaming_active()) {
        obs_frontend_streaming_stop();
    } else {
        ec = make_error_code(HandleError::StreamingError);
    }
}

void EventHandler::recording_split_file(std::string& msg, handle_error& ec) {
    (void)(msg);
    if (obs_frontend_recording_active()) {
        obs_frontend_recording_split_file();
    } else {
        ec = make_error_code(HandleError::RecordingError);
    }
}
