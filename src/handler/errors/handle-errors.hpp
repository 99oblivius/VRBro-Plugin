#pragma once

#include <string>
#include <system_error>

using handle_error = std::error_code;

enum class HandleError {
    Success = 0,
    EmptyMessage,
    UnknownCommand,
    ReplayBufferError,
    RecordingError,
    StreamingError,
    SceneError
};

class HandleErrorCategory final : public std::error_category {
public:
    const char* name() const noexcept override {
        return "HandleError";
    }

    std::string message(int ev) const override {
        switch (static_cast<HandleError>(ev)) {
            case HandleError::Success:
                return "Success";
            case HandleError::EmptyMessage:
                return "Empty message";
            case HandleError::UnknownCommand:
                return "Unknown command";
            case HandleError::ReplayBufferError:
                return "Replay buffer error";
            case HandleError::RecordingError:
                return "Recording error";
            case HandleError::StreamingError:
                return "Streaming error";
            case HandleError::SceneError:
                return "Scene Error";
            default:
                return "Unknown error";
        }
    }
};

const std::error_category& getHandleErrorCategory();
std::error_code make_error_code(HandleError e);
