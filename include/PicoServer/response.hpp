#ifndef PS_RESPONSE_HPP
#define PS_RESPONSE_HPP

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include "PicoServer/config.hpp"
#include "PicoServer/non_copyable.hpp"

namespace ps
{
    enum class status_code;

    class PS_API response : public virtual non_copyable
    {
    public:
        response() = delete;

        response(const status_code status_code, std::map<std::string, std::string> headers,
                 std::optional<std::string> body) :
            status_code_{status_code},
            headers_{std::move(headers)},
            body_{std::move(body)}
        {
        }

        virtual ~response() = default;

        response(const response& response)
        {
            status_code_ = response.status_code_;
            body_ = response.body_;
        }

        response& operator=(response&& response) noexcept
        {
            status_code_ = response.status_code_;
            headers_ = std::move(response.headers_);
            body_ = std::move(response.body_);

            return *this;
        }

        status_code get_status_code() const { return status_code_; }

        const std::map<std::string, std::string>& get_headers() const { return headers_; }

        const std::optional<std::string>& get_body() const { return body_; }

    private:
        status_code status_code_;

        std::map<std::string, std::string> headers_;

        std::optional<std::string> body_;
    };
}

#endif
