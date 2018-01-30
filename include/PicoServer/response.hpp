#ifndef PS_RESPONSE_HPP
#define PS_RESPONSE_HPP

#include <memory>
#include <sstream>
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

        response(const status_code status_code, std::string content) :
            status_code_{status_code},
            content_{std::move(content)}
        {
        }

        virtual ~response() = default;

        response(const response& response)
        {
            status_code_ = response.status_code_;
            content_ = response.content_;
        }

        response& operator=(response&& response) noexcept
        {
            status_code_ = response.status_code_;
            content_ = std::move(response.content_);

            return *this;
        }

        status_code get_status_code() const { return status_code_; }

        const std::string& get_content() const { return content_; }

        void set_status_code(const status_code status_code) { status_code_ = status_code; }

        void set_content(const std::string& content) { content_ = content; }

    private:
        status_code status_code_;

        std::string content_;
    };
}

#endif
