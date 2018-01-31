#include "PicoServer/server.hpp"
#include <chrono>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>
#ifdef PS_WINDOWS
#  include <WS2tcpip.h>
#endif
#include "PicoServer/request.hpp"
#include "PicoServer/response.hpp"
#include "PicoServer/status_code.hpp"
#include "PicoServer/utils.hpp"
#include "socket_impl.hpp"

using namespace ps;

namespace priv
{
    void parse_request_line(socket_handle socket, std::string& method, std::string& uri, std::string& http_version);

    void parse_headers(socket_handle socket, std::map<std::string, std::string>& headers);

    void parse_body(socket_handle socket, std::vector<char>& body, size_t length);

    request receive_request(socket_handle socket, const std::string& remote_ip, uint16_t remote_port);

    void send_response(socket_handle socket, const response& response);

#ifdef PS_WINDOWS
    constexpr auto flags = 0;
#else
    constexpr auto flags = MSG_NOSIGNAL;
#endif
}

namespace ps
{
    server::server(const uint16_t port, const uint32_t backlog) :
        port_{port},
        listener_{socket_impl::invalid_socket},
        running_{false}
    {
        addrinfo hints{}, *info, *ptr;

        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        if (getaddrinfo(nullptr, std::to_string(port_).c_str(), &hints, &info) != 0)
            throw std::runtime_error("getaddrinfo failed: " + socket_impl::get_error());

        for (ptr = info; ptr != nullptr; ptr = ptr->ai_next)
        {
            listener_ = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
            if (listener_ == socket_impl::invalid_socket)
            {
                std::cerr << "socket failed: " << socket_impl::get_error() << std::endl;
                continue;
            }

#ifdef PS_WINDOWS
            auto constexpr on = '1';
#else
            auto constexpr on = 1;
#endif
            if (setsockopt(listener_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on) == socket_impl::socket_error)
                throw std::runtime_error("setsockopt failed: " + socket_impl::get_error());

            if (bind(listener_, ptr->ai_addr, ptr->ai_addrlen) == socket_impl::socket_error)
            {
                std::cerr << "bind failed: " << socket_impl::get_error() << std::endl;
                socket_impl::close(listener_);
                continue;
            }

            break;
        }

        freeaddrinfo(info);

        if (ptr == nullptr) throw std::runtime_error("bind failed");

        if (listen(listener_, backlog) == socket_impl::socket_error)
            throw std::runtime_error("listen failed: " + socket_impl::get_error());
    }

    server::~server()
    {
        if (listener_ != socket_impl::invalid_socket)
            socket_impl::close(listener_);
    }

    void server::add_default_route(const std::function<response(const request&)>& func)
    {
        default_route_ = func;
    }

    void server::map_get_route(const std::string& template_name, const std::function<response(const request&)>& func)
    {
        get_routes_[template_name] = func;
    }

    void server::map_post_route(const std::string& template_name, const std::function<response(const request&)>& func)
    {
        post_routes_[template_name] = func;
    }

    void server::start()
    {
        running_ = true;

        while (running_)
        {
            sockaddr_storage connector_addr{};
#ifdef PS_WINDOWS
            int length = sizeof connector_addr;
#else
            socklen_t length = sizeof connectorAddr;
#endif
            auto connection_socket = accept(listener_, reinterpret_cast<sockaddr*>(&connector_addr), &length);

            if (connection_socket == socket_impl::invalid_socket)
                throw std::runtime_error("accept failed: " + socket_impl::get_error());

            const auto sa = reinterpret_cast<sockaddr *>(&connector_addr);
            void* in_addr;
            uint16_t in_port;
            if (sa->sa_family == AF_INET)
            {
                in_addr = &reinterpret_cast<sockaddr_in*>(sa)->sin_addr;
                in_port = reinterpret_cast<sockaddr_in*>(sa)->sin_port;
            }
            else
            {
                in_addr = &reinterpret_cast<sockaddr_in6*>(sa)->sin6_addr;
                in_port = reinterpret_cast<sockaddr_in6*>(sa)->sin6_port;
            }

            char ip[INET6_ADDRSTRLEN];
            if (inet_ntop(connector_addr.ss_family, in_addr, ip, sizeof ip) == nullptr)
                throw std::runtime_error("inet_ntop failed: " + socket_impl::get_error());

            std::thread(&server::run, this, connection_socket, std::string(ip), ntohs(in_port)).detach();
        }
    }

    void server::stop()
    {
        running_ = false;
    }

    void server::run(const socket_handle socket, const std::string& ip, const uint16_t port) const
    {
        std::optional<std::function<response(const request&)>> func;
        auto r = priv::receive_request(socket, ip, port);

        if (r.get_method() == "GET" || r.get_method() == "POST")
        {
            auto routes = r.get_method() == "GET" ? get_routes_ : post_routes_;
            for (const auto& route : routes)
            {
                std::smatch sm;
                if (std::regex_match(r.get_uri(), sm, std::regex(route.first)))
                {
                    std::vector<std::string> matches;
                    for (const auto& match : sm)
                        matches.push_back(match);
                    r = request(r.get_remote_ip(), r.get_remote_port(),
                                r.get_method(), r.get_uri(), r.get_http_version(),
                                r.get_headers(),
                                r.get_body(),
                                matches);
                    func = route.second;
                    break;
                }
            }
        }
        else
        {
            // TODO
        }

        if (func)
        {
            const auto response = (*func)(r);
            priv::send_response(socket, response);
        }

        socket_impl::close(socket);
    }
}

namespace priv
{
    void parse_request_line(
        const socket_handle socket,
        std::string& method,
        std::string& uri,
        std::string& http_version)
    {
        std::optional<char> last;
        auto method_flag = true;
        std::vector<char> buffer;

        while (true)
        {
            char current;
            const auto size_received = recv(socket, &current, 1, flags);

            if (size_received == socket_impl::socket_error)
                throw std::runtime_error("recv failed: " + socket_impl::get_error());
            if (size_received == 0)
                throw std::runtime_error("parse_request_line failed: recv returned 0");

            if (current == ' ')
            {
                if (method_flag)
                {
                    method = std::string(buffer.data(), buffer.size());
                    buffer.clear();
                    method_flag = false;
                }
                else
                {
                    uri = std::string(buffer.data(), buffer.size());
                    buffer.clear();
                }
            }
            else if (last == '\r' && current == '\n')
            {
                buffer.pop_back(); // pop '\r'
                http_version = std::string(buffer.data(), buffer.size());

                break;
            }
            else buffer.push_back(current);

            last = current;
        }
    }

    void parse_headers(const socket_handle socket, std::map<std::string, std::string>& headers)
    {
        std::optional<char> last;
        auto field_flag = true;
        std::string field_name;
        std::vector<char> buffer;

        while (true)
        {
            char current;
            const auto size_received = recv(socket, &current, 1, flags);

            if (size_received == socket_impl::socket_error)
                throw std::runtime_error("recv failed: " + socket_impl::get_error());
            if (size_received == 0)
                throw std::runtime_error("parse_headers failed: recv returned 0");

            if (current == ':' && field_flag)
            {
                field_name = utils::trim(std::string(buffer.data(), buffer.size()));
                buffer.clear();
                field_flag = false;
            }
            else if (last == '\r' && current == '\n')
            {
                if (buffer.size() == 1 && buffer.back() == '\r') break;
                buffer.pop_back(); // pop '\r'
                headers[field_name] = utils::trim(std::string(buffer.data(), buffer.size()));
                buffer.clear();
                field_name.clear();
                field_flag = true;
            }
            else buffer.push_back(current);

            last = current;
        }
    }

    void parse_body(const socket_handle socket, std::vector<char>& body, const size_t length)
    {
        constexpr size_t buffer_size = 1024;
        size_t all_size_received = 0;

        while (all_size_received < length)
        {
            char buffer[buffer_size];
            const auto size_received = recv(socket, buffer, buffer_size, flags);

            if (size_received == socket_impl::socket_error)
                throw std::runtime_error("recv failed: " + socket_impl::get_error());
            if (size_received == 0)
                throw std::runtime_error("parse_body failed: recv returned 0");

            all_size_received += size_received;
            body.insert(body.end(), buffer, buffer + size_received);
        }
    }

    request receive_request(const socket_handle socket, const std::string& remote_ip, const uint16_t remote_port)
    {
        std::string method;
        std::string uri;
        std::string http_verson;
        parse_request_line(socket, method, uri, http_verson);

        std::map<std::string, std::string> headers;
        parse_headers(socket, headers);

        std::vector<char> body;
        const auto it = headers.find("Content-Length");
        if (it != headers.end())
        {
            size_t length;
            std::stringstream(it->second) >> length;

            parse_body(socket, body, length);
        }
        const auto optional_body = !body.empty() ? body : std::optional<std::vector<char>>{};

        return request(remote_ip, remote_port, method, uri, http_verson, headers, optional_body, std::nullopt);
    }

    void send_response(const socket_handle socket, const response& response)
    {
        const auto code = static_cast<std::underlying_type_t<status_code>>(response.get_status_code());
        const auto code_phrase = utils::get_reason_phrase(response.get_status_code());

        std::stringstream stream;
        stream << "HTTP/1.1 " + std::to_string(code) + " " + code_phrase + "\r\n";
        for (const auto& header : response.get_headers())
            stream << header.first << ": " << header.second << "\r\n";
        stream << "\r\n";
        if (response.get_body()) stream << *response.get_body();
        const auto response_str = stream.str();

        size_t all_size_sent = 0;
        const auto length = response_str.length();
        auto bytes_left = length;

        while (all_size_sent < length)
        {
            const auto size_sent = send(socket, response_str.c_str() + all_size_sent, bytes_left, 0);
            if (size_sent == socket_impl::socket_error)
                throw std::runtime_error("send failed: " + socket_impl::get_error());

            all_size_sent += size_sent;
            bytes_left -= size_sent;
        }
    }
}
