#include "lib2.h"

namespace lib2 {

const char* ErrorCategory::name() const noexcept { return "lib-2"; }
std::string ErrorCategory::message(int ev) const {
    switch (static_cast<Error>(ev)) {
        case Error::kNotImplemented:
            return "not implemented";
        default:
            return "unknown";
    }
}
const std::error_category& get_error_category() {
    const static ErrorCategory sErrorCategory;
    return sErrorCategory;
}
std::error_code make_error_code(Error ev) {
    return std::error_code(static_cast<int>(ev), get_error_category());
}

}  // end namespace
