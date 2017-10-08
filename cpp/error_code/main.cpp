/**
 * @author visaodumuc
 * @brief Ví dụ về sử dụng std::error_code
 * Mã lỗi ở 1 thư viện lib1 và lib2 tuy có cùng giá trị (value) 
 * nhưng có thể được so sánh khác nhau, từ đó phân biệt được lỗi
 * ở thư viện 1 hay thư viện 2 khi nhận được mã lỗi error_code.
 *
 * Compile: g++ --std=c++11 main.cpp lib1.cpp lib2.cpp -o test
 */

#include "lib1.h"
#include "lib2.h"
#include <iostream>

int main(int argc, char **argv) {
    {
        std::cout << "TEST lib 1" << std::endl;
        std::error_code err = lib1::Error::kNotImplemented;
        std::cout << "is lib-1 error: " << (err == lib1::Error::kNotImplemented) << std::endl;
        std::cout << "is lib-2 error: " << (err == lib2::Error::kNotImplemented) << std::endl;
        std::cout << "value    = " << err.value() << std::endl;
        std::cout << "message  = " << err.message() << std::endl;
        std::cout << "category = " << err.category().name() << std::endl;
    }
    {
        std::cout << std::endl;
        std::cout << "TEST lib 2" << std::endl;
        std::error_code err = lib2::Error::kNotImplemented;
        std::cout << "is lib-1 error: " << (err == lib1::Error::kNotImplemented) << std::endl;
        std::cout << "is lib-2 error: " << (err == lib2::Error::kNotImplemented) << std::endl;
        std::cout << "value    = " << err.value() << std::endl;
        std::cout << "message  = " << err.message() << std::endl;
        std::cout << "category = " << err.category().name() << std::endl;
    }

    return 0;
}
