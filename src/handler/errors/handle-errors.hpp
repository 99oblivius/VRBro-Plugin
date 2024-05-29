#ifndef HANDLE_ERRORS_HPP
#define HANDLE_ERRORS_HPP

#include <system_error>

typedef std::error_code handle_error;

enum class HandleError {
    Success = 0,
    EmptyMessage,
    UnknownCommand,
    ReplayBufferError,
    RecordingError,
    StreamingError,
};

class HandleErrorCategory : public std::error_category {
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
            default:
                return "Unknown error";
        }
    }
};

static const std::error_category& getHandleErrorCategory() {
    static HandleErrorCategory instance;
    return instance;
}

static std::error_code make_error_code(HandleError e) {
    return {static_cast<int>(e), getHandleErrorCategory()};
}

#endif