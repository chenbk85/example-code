#ifndef __VSDM_TYPEINFO_H__
#define __VSDM_TYPEINFO_H__

#include <cxxabi.h>
#include <string>
#include <typeinfo>

namespace vsdm {
template <typename T>
struct typeinfo {
    static const std::string name() {
        std::string ret = typeid(T).name();
        char *csz = abi::__cxa_demangle(ret.c_str(), 0, 0, 0);
        if (csz) {
            ret = std::string(csz);
            free(csz);
        }
        return ret;
    }
};
}

#endif