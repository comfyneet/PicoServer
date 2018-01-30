#ifndef PS_UTILS_HPP
#define PS_UTILS_HPP

#include <string>
#include "PicoServer/config.hpp"

namespace ps
{
    enum class status_code;

    class PS_API utils
    {
    public:
        static std::string trim(const std::string& str);

        static std::string get_reason_phrase(status_code status_code);

        utils() = delete;
    };
}

#endif
