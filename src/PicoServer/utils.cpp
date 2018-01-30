#include "PicoServer/utils.hpp"
#include <unordered_map>
#include "PicoServer/status_code.hpp"

namespace priv
{
    const std::unordered_map<ps::status_code, std::string> codes =
    {
        {ps::status_code::continue_, "Continue"},
        {ps::status_code::switching_protocols, "Switching Protocols"},
        {ps::status_code::ok, "OK"},
        {ps::status_code::created, "Created"},
        {ps::status_code::accepted, "Accepted"},
        {ps::status_code::non_authoritative_information, "Non-Authoritative Information"},
        {ps::status_code::no_content, "No Content"},
        {ps::status_code::reset_content, "Reset Content"},
        {ps::status_code::partial_content, "Partial Content"},
        {ps::status_code::multiple_choices, "Multiple Choices"},
        {ps::status_code::moved_permanently, "Moved Permanently"},
        {ps::status_code::found, "Found"},
        {ps::status_code::see_other, "See Other"},
        {ps::status_code::not_modified, "Not Modified"},
        {ps::status_code::use_proxy, "Use Proxy"},
        {ps::status_code::temporary_redirect, "Temporary Redirect"},
        {ps::status_code::bad_request, "Bad Request"},
        {ps::status_code::unauthorized, "Unauthorized"},
        {ps::status_code::payment_required, "Payment Required"},
        {ps::status_code::forbidden, "Forbidden"},
        {ps::status_code::not_found, "Not Found"},
        {ps::status_code::method_not_allowed, "Method Not Allowed"},
        {ps::status_code::not_acceptable, "Not Acceptable"},
        {ps::status_code::proxy_authentication_required, "Proxy Authentication Required"},
        {ps::status_code::request_time_out, "Request Time-out"},
        {ps::status_code::conflict, "Conflict"},
        {ps::status_code::gone, "Gone"},
        {ps::status_code::length_required, "Length Required"},
        {ps::status_code::precondition_failed, "Precondition Failed"},
        {ps::status_code::request_entity_too_large, "Request Entity Too Large"},
        {ps::status_code::request_uri_too_large, "Request-URI Too Large"},
        {ps::status_code::unsupported_media_type, "Unsupported Media Type"},
        {ps::status_code::requested_range_not_satisfiable, "Requested range not satisfiable"},
        {ps::status_code::expectation_failed, "Expectation Failed"},
        {ps::status_code::internal_server_error, "Internal Server Error"},
        {ps::status_code::not_implemented, "Not Implemented"},
        {ps::status_code::bad_gateway, "Bad Gateway"},
        {ps::status_code::service_unavailable, "Service Unavailable"},
        {ps::status_code::gateway_time_out, "Gateway Time-out"},
        {ps::status_code::http_version_not_supported, "HTTP Version not supported"},
    };
}

namespace ps
{
    std::string utils::trim(const std::string& str)
    {
        const auto first = str.find_first_not_of(' ');

        if (std::string::npos == first) return str;

        const auto last = str.find_last_not_of(' ');

        return str.substr(first, last - first + 1);
    }

    std::string utils::get_reason_phrase(const status_code status_code)
    {
        return priv::codes.find(status_code)->second;
    }
}
