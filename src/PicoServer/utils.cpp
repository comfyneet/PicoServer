#include "PicoServer/utils.hpp"

namespace ps
{
    std::string utils::trim(const std::string& str)
    {
        const auto first = str.find_first_not_of(' ');

        if (std::string::npos == first) return str;

        const auto last = str.find_last_not_of(' ');

        return str.substr(first, last - first + 1);
    }
}
