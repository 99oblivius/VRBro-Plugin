#include <string>

namespace Utils {
std::string serializeCharArrays(char** arrays) {
    if (!arrays) return "";
    
    std::string result;
    
    size_t totalSize = 0;
    for (char** ptr = arrays; *ptr != nullptr; ++ptr)
        totalSize += strlen(*ptr) + 1;
    
    result.reserve(totalSize);
    
    for (char** ptr = arrays; *ptr != nullptr; ++ptr) {
        if (!result.empty()) {
            result += '\0';
        }
        result += *ptr;
    }
    
    return result;
}
} // namespace Utils