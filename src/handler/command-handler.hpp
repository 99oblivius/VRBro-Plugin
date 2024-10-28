#pragma once

#include <cstdint>
#include <string_view>
#include <system_error>
#include <vector>

namespace CommandHandler {
    void Handler(std::string_view packet, std::vector<uint8_t>& response, std::error_code& ec);
}
