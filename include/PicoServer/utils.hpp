#ifndef PS_UTILS_HPP
#define PS_UTILS_HPP

#include <string>
#include "PicoServer/config.hpp"

namespace ps
{
    class PS_API utils
    {
    public:
        static std::string trim(const std::string& str);

        utils() = delete;
    };
}

#endif
