#ifndef PS_REQUEST_HPP
#define PS_REQUEST_HPP

#include <map>
#include <optional>
#include <vector>
#include "PicoServer/config.hpp"
#include "PicoServer/non_copyable.hpp"

namespace ps
{
    class PS_API request : public non_copyable
    {
    public:
        request() = delete;

        request(
            const std::string& method,
            const std::string& uri,
            const std::string& http_version,
            const std::map<std::string, std::string>& headers,
            const std::optional<std::vector<char>>& body,
            const std::optional<std::vector<std::string>>& uri_matches = std::nullopt) :
            method_{ method },
            uri_{ uri },
            http_version_{ http_version },
            headers_{ headers },
            body_{ body },
            uri_matches_{ uri_matches }
        {
        }

        request(const request& request) :
            method_{ request.method_ },
            uri_{ request.uri_ },
            http_version_{ request.http_version_ },
            headers_{ request.headers_ },
            body_{ request.body_ },
            uri_matches_{ request.uri_matches_ }
        {
        }

        const std::string& get_method() const { return method_; }

        const std::string& get_uri() const { return uri_; }

        const std::string& get_http_version() const { return http_version_; }

        const std::map<std::string, std::string>& get_headers() const { return headers_; }

        std::optional<std::string> get_header(const std::string& field);

        const std::optional<std::vector<char>>& get_body() const { return body_; }

        void set_uri_matches(const std::vector<std::string>& uri_matches) { uri_matches_ = uri_matches; }

    private:
        std::string method_;

        std::string uri_;

        std::string http_version_;

        std::map<std::string, std::string> headers_;

        std::optional<std::vector<char>> body_;

        std::optional<std::vector<std::string>> uri_matches_;
    };
}

#endif
