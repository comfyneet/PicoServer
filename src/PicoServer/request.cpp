#include "PicoServer/request.hpp"

namespace ps
{
    std::optional<std::string> request::get_header(const std::string& field)
    {
        auto it = headers_.find(field);

        return it != headers_.end() ? it->second : std::optional<std::string>{};
    }
}
