#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <iostream>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include "events/action-type.hpp"
#include "events/event-handlers.hpp"
#include "events/request-handlers.hpp"

namespace CommandHandler {
    void Handler(std::string_view& packet, std::vector<uint8_t>& response, std::error_code& ec);

    namespace {
        void unknown_command(std::string& msg, std::error_code& ec) {
            ec = make_error_code(HandleError::UnknownCommand); 
        }

        void ping(std::string& msg, std::error_code& ec) {
            msg = "pong";
        }

        void RequestHandler(std::string_view& packet, ActionType& action, std::string& msg, std::error_code& ec) {
            action = static_cast<uint8_t>(packet[0]) & 0x3F;
            switch (static_cast<ActionTypeRequest>(action)) {
                case ActionTypeRequest::PING:
                    ping(msg, ec); break;
                case ActionTypeRequest::REPLAY_BUFFER_ACTIVE:
                    RequestHandler::is_replay_buffer(msg, ec); break;
                case ActionTypeRequest::RECORDING_ACTIVE:
                    RequestHandler::is_recording(msg, ec); break;
                case ActionTypeRequest::STREAMING_ACTIVE:
                    RequestHandler::is_streaming(msg, ec); break;
                default:
                    unknown_command(msg, ec); break;
            }
        }

        void EventHandler(std::string_view& packet, ActionType& action, std::string& msg, std::error_code& ec) {
            action = static_cast<uint8_t>(packet[0]) & 0x3F;
            switch (static_cast<ActionTypeEvent>(action)) {
                case ActionTypeEvent::PING:
                    ping(msg, ec); break;
                case ActionTypeEvent::START_REPLAY_BUFFER:
                    EventHandler::start_replay_buffer(msg, ec); break;
                case ActionTypeEvent::STOP_REPLAY_BUFFER:
                    EventHandler::stop_replay_buffer(msg, ec); break;
                case ActionTypeEvent::SAVE_REPLAY_BUFFER:
                    EventHandler::save_replay_buffer(msg, ec); break;
                case ActionTypeEvent::START_RECORDING:
                    EventHandler::start_recording(msg, ec); break;
                case ActionTypeEvent::STOP_RECORDING:
                    EventHandler::stop_recording(msg, ec); break;
                case ActionTypeEvent::START_STREAMING:
                    EventHandler::start_streaming(msg, ec); break;
                case ActionTypeEvent::STOP_STREAMING:
                    EventHandler::stop_streaming(msg, ec); break;
                case ActionTypeEvent::RECORDING_SPLIT_FILE:
                    EventHandler::recording_split_file(msg, ec); break;
                default:
                    unknown_command(msg, ec); break;
            }
        }
    }
};

#endif
