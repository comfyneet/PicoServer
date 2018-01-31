#ifndef PS_REQUEST_HPP
#define PS_REQUEST_HPP

#include <map>
#include <optional>
#include <utility>
#include <vector>
#include "PicoServer/config.hpp"
#include "PicoServer/non_copyable.hpp"

namespace ps
{
    class PS_API request : public virtual non_copyable
    {
    public:
        request() = delete;

        request(
            std::string remote_ip,
            const uint16_t remote_port,
            std::string method,
            std::string uri,
            std::string http_version,
            std::map<std::string, std::string> headers,
            std::optional<std::vector<char>> body,
            std::optional<std::vector<std::string>> uri_matches) :
            remote_ip_{std::move(remote_ip)},
            remote_port_{remote_port},
            method_{std::move(method)},
            uri_{std::move(uri)},
            http_version_{std::move(http_version)},
            headers_{std::move(headers)},
            body_{std::move(body)},
            uri_matches_{std::move(uri_matches)}
        {
        }

        request(const request& request) :
            remote_ip_{request.remote_ip_},
            remote_port_{request.remote_port_},
            method_{request.method_},
            uri_{request.uri_},
            http_version_{request.http_version_},
            headers_{request.headers_},
            body_{request.body_},
            uri_matches_{request.uri_matches_}
        {
        }

        virtual ~request() = default;

        request& operator=(request&& request) noexcept
        {
            remote_ip_ = std::move(request.remote_ip_);
            remote_port_ = request.remote_port_;
            method_ = std::move(request.method_);
            uri_ = std::move(request.uri_);
            http_version_ = std::move(request.http_version_);
            headers_ = std::move(request.headers_);
            body_ = std::move(request.body_);
            uri_matches_ = std::move(request.uri_matches_);

            return *this;
        }

        const std::string& get_remote_ip() const { return remote_ip_; }

        uint16_t get_remote_port() const { return remote_port_; }

        const std::string& get_method() const { return method_; }

        const std::string& get_uri() const { return uri_; }

        const std::string& get_http_version() const { return http_version_; }

        const std::map<std::string, std::string>& get_headers() const { return headers_; }

        const std::optional<std::vector<char>>& get_body() const { return body_; }

        std::optional<std::string> get_uri_match(uint32_t id) const;

    private:
        std::string remote_ip_;

        uint16_t remote_port_;

        std::string method_;

        std::string uri_;

        std::string http_version_;

        std::map<std::string, std::string> headers_;

        std::optional<std::vector<char>> body_;

        std::optional<std::vector<std::string>> uri_matches_;
    };
}

#endif
