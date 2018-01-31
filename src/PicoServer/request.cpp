#include "PicoServer/request.hpp"

namespace ps
{
    std::optional<std::string> request::get_uri_match(const uint32_t id) const
    {
        return uri_matches_ && id < (*uri_matches_).size() ? (*uri_matches_)[id] : std::optional<std::string>{};
    }
}
