#include "handle-errors.hpp"

namespace std {
    template <>
    struct is_error_code_enum<HandleError> : true_type {};
}

const std::error_category& getHandleErrorCategory() {
    static HandleErrorCategory instance;
    return instance;
}

std::error_code make_error_code(HandleError e) {
    return {static_cast<int>(e), getHandleErrorCategory()};
}