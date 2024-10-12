#pragma once

typedef uint8_t ActionType;

enum class ActionTypeEvent : uint8_t {
    PING = 0,
    START_REPLAY_BUFFER = 1,
    STOP_REPLAY_BUFFER = 2,
    SAVE_REPLAY_BUFFER = 3,
    START_RECORDING = 4,
    STOP_RECORDING = 5,
    START_STREAMING = 6,
    STOP_STREAMING = 7,
    RECORDING_SPLIT_FILE = 8,
};

enum class ActionTypeRequest : uint8_t {
    PING = 0,
    REPLAY_BUFFER_ACTIVE = 1,
    RECORDING_ACTIVE = 2,
    STREAMING_ACTIVE = 3,
};
