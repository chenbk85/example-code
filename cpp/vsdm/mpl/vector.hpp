#ifndef __VSDM_VECTOR_H__
#define __VSDM_VECTOR_H__
/**
 * @author visaodumuc
 * @brief MPL Meta-Programing Library
 */

#include <cstdlib>

namespace vsdm {
namespace mpl {

namespace _vector_ {
template <std::size_t index, class... Args>
struct at {};

template <std::size_t index, class Head, class... Tail>
struct at<index, Head, Tail...> {
    typedef typename at<index - 1, Tail...>::type type;
};

template <class Head, class... Tail>
struct at<0, Head, Tail...> {
    typedef Head type;
};
}

template <typename... Args>
struct vector {
    using type = vector<Args...>;
    // number of elements in vector
    enum {
        size = sizeof...(Args),
    };
    // get elements at index
    template <std::size_t index>
    struct at {
        using type = typename _vector_::at<index, Args...>::type;
    };
};
}
}

#endif