#include "../../typeinfo.hpp"
#include "../vector.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <typeinfo>

namespace {
using vec = typename vsdm::mpl::vector<int, char, float>::type;
}

TEST(mpl, vector_size) {
    static_assert(vec::size == 3);
}
TEST(mpl, vector_at) {
    static_assert(std::is_same<int, vec::at<0>::type>::value);
    std::cout << vsdm::typeinfo<vec>::name() << std::endl;
}