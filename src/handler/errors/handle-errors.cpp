#include "handle-errors.hpp"

namespace std {
    template <>
    struct is_error_code_enum<HandleError> : true_type {};
}
