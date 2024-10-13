#include "errors/handle-errors.hpp"
#include "events/event-handlers.hpp"
#include "events/request-handlers.hpp"
#include "handler.hpp"

void CommandHandler::Handler(std::string_view& packet, std::vector<uint8_t>& response, std::error_code& ec) {
    ActionType action = 0xFF;
    std::string msg = "";

    uint8_t header = 0;

    if (packet.empty()) {
        action = 0xFF;
        ec = make_error_code(HandleError::EmptyMessage);
    } else {
        std::string payload;
        if (packet.size() > 1) {
            payload.assign(packet.begin() + 1, packet.end());
        }
        action = packet[0] & 0x3F;
        if ((uint8_t)packet[0] & 0x80) {
            EventHandler(packet, action, msg, ec);
            header |= 1 << 7;
        } else {
            RequestHandler(packet, action, msg, ec);
        }
    }

    header |= ((msg.empty() ? 0 : 1) | (bool)ec ? 0 : 1) << 6;
    header |= static_cast<uint8_t>(action) & 0x3F;
    response.push_back(header);
    
    if (!msg.empty()) {
        response.insert(response.end(), msg.begin(), msg.end());
    }
}
