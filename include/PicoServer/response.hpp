#ifndef PS_RESPONSE_HPP
#define PS_RESPONSE_HPP

#include <optional>
#include <sstream>
#include <string>
#include "PicoServer/config.hpp"
#include "PicoServer/non_copyable.hpp"

namespace ps
{
    class PS_API response : public non_copyable
    {
    public:
        static std::optional<std::string> get_reason_phrase(int32_t status_code);

        response() :
            status_code_{}
        {
        }

        void append(const std::string& content);

    private:
        std::string http_version_;

        int32_t status_code_;

        std::stringstream stream_;
    };
}

#endif
