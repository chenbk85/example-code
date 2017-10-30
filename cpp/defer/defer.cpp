//
// @author binhfile
// g++ -std=c++11 -O2 defer.cpp -o defer
//

#include <iostream>

namespace __hidden__ {
template <typename Lambda> class defer_impl {
  public:
    defer_impl(const Lambda &expr) : expr_(expr) {}
    virtual ~defer_impl() { expr_(); }

  private:
    Lambda expr_;
};
} // namespace __hidden__
// clang-format off
#define TOKEN_CONCAT(x, y) x ## y
#define DEFER_IMPL(lname, vname, expr)                      \
    auto lname = expr;                                      \
    __hidden__::defer_impl<decltype(lname)> vname(lname);
#define DEFER_INDEX(index, expr) DEFER_IMPL(TOKEN_CONCAT(_hidden_defer_lambda_, index), TOKEN_CONCAT(_hidden_defer_impl_, index), expr)
#define DEFER(expr) DEFER_INDEX(__COUNTER__, expr)
// clang-format on

int main(int argc, const char **args) {
    DEFER([] { std::cout << "app - defer" << std::endl; })
    std::cout << "app - start" << std::endl;
    {
        int index = 1;
        DEFER([&index] { std::cout << index << " - defer" << std::endl; })
        std::cout << index << " - enter" << std::endl;

        std::cout << index << " - exit" << std::endl;
    }
    {
        int index = 2;
        DEFER([&index] { std::cout << index << " - defer 1 " << std::endl; })
        DEFER([&index] { std::cout << index << " - defer 2" << std::endl; })
        std::cout << index << " - enter" << std::endl;

        std::cout << index << " - exit" << std::endl;
    }
    std::cout << "app - stop" << std::endl;
    return 0;
}
