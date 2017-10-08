#ifndef __LIB2_H__
#define __LIB2_H__

#include <system_error>

namespace lib2 {

// Định nghĩa các mã lõi trong thư viện
enum class Error {
    kUnknown = 1,
    kNotImplemented,
};
class ErrorCategory : public std::error_category {
   public:
    const char* name() const noexcept override;
    std::string message(int ev) const override;
};
const std::error_category& get_error_category();
std::error_code make_error_code(Error ev);

}  // end namespace

// Đăng ký mã lỗi với std
namespace std {
template <>
struct is_error_code_enum<lib2::Error> : public std::true_type {};
}

#endif
