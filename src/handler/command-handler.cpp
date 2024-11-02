#include "command-handler.hpp"

#include "errors/handle-errors.hpp"
#include "events/action-type.hpp"
#include "events/event-handlers.hpp"
#include "events/request-handlers.hpp"

namespace {
    void ping(std::string& msg, std::error_code& ec) {
        ec.clear();
        msg = "pong";
    }

    void unknown_command(std::string& msg, std::error_code& ec) {
        msg.clear();
        ec = make_error_code(HandleError::UnknownCommand); 
    }

    void handle_request(std::string_view& packet, ActionType& action, std::string& msg, std::error_code& ec) {
        action = static_cast<uint8_t>(packet[0]) & 0x3F;
        switch (static_cast<ActionTypeRequest>(action)) {
            case ActionTypeRequest::PING:
                ping(msg, ec);
                break;
            case ActionTypeRequest::REPLAY_BUFFER_ACTIVE:
                RequestHandler::is_replay_buffer(msg, ec);
                break;
            case ActionTypeRequest::RECORDING_ACTIVE:
                RequestHandler::is_recording(msg, ec);
                break;
            case ActionTypeRequest::STREAMING_ACTIVE:
                RequestHandler::is_streaming(msg, ec);
                break;
            case ActionTypeRequest::GET_CURRENT_SCENE:
                RequestHandler::get_current_scene(msg, ec);
                break;
            case ActionTypeRequest::GET_SCENES:
                RequestHandler::get_scenes(msg, ec);
                break;
            default:
                unknown_command(msg, ec);
                break;
        }
    }

    void handle_event(std::string_view& packet, ActionType& action, std::string& msg, std::error_code& ec) {
        action = static_cast<uint8_t>(packet[0]) & 0x3F;
        std::string payload;
        if (packet.length() > 1) {
            payload = std::string(packet.substr(1));
        }
        switch (static_cast<ActionTypeEvent>(action)) {
            case ActionTypeEvent::PING:
                ping(msg, ec);
                break;
            case ActionTypeEvent::START_REPLAY_BUFFER:
                EventHandler::start_replay_buffer(msg, ec);
                break;
            case ActionTypeEvent::STOP_REPLAY_BUFFER:
                EventHandler::stop_replay_buffer(msg, ec);
                break;
            case ActionTypeEvent::SAVE_REPLAY_BUFFER:
                EventHandler::save_replay_buffer(msg, ec);
                break;
            case ActionTypeEvent::START_RECORDING:
                EventHandler::start_recording(msg, ec);
                break;
            case ActionTypeEvent::STOP_RECORDING:
                EventHandler::stop_recording(msg, ec);
                break;
            case ActionTypeEvent::START_STREAMING:
                EventHandler::start_streaming(msg, ec);
                break;
            case ActionTypeEvent::STOP_STREAMING:
                EventHandler::stop_streaming(msg, ec);
                break;
            case ActionTypeEvent::RECORDING_SPLIT_FILE:
                EventHandler::recording_split_file(msg, ec);
                break;
            case ActionTypeEvent::SET_SCENE:
                EventHandler::set_scene(payload, ec);
                break;
            default:
                unknown_command(msg, ec);
                break;
        }
    }
}

namespace CommandHandler {
    void Handler(std::string_view packet, std::vector<uint8_t>& response, std::error_code& ec) {
        ActionType action = 0xFF;
        std::string msg;

        uint8_t header = 0;
        uint8_t success_flag = 0;

        if (packet.empty()) {
            action = 0xFF;
            ec = make_error_code(HandleError::EmptyMessage);
        } else {
            std::string payload;
            if (packet.size() > 1) {
                payload.assign(packet.begin() + 1, packet.end());
            }
            action = packet[0] & 0x3F;
            if (static_cast<uint8_t>(packet[0]) & 0x80) {
                handle_event(packet, action, msg, ec);
                header |= 1 << 7;
                success_flag = ec ? 0 : 1;
            } else {
                handle_request(packet, action, msg, ec);
                success_flag = msg.empty() ? 0 : 1;
            }
        }

        header |= success_flag << 6;
        
        header |= static_cast<uint8_t>(action) & 0x3F;
        response.push_back(header);
        
        if (!msg.empty()) {
            response.insert(response.end(), msg.begin(), msg.end());
        }
    }
}