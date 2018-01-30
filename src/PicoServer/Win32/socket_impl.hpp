#ifndef PS_WIN32_SOCKET_IMPL_HPP
#define PS_WIN32_SOCKET_IMPL_HPP

#include <string>
#include "PicoServer/socket_handle.hpp"

namespace ps
{
    class socket_impl
    {
    public:
        typedef int address_length;

        static constexpr socket_handle invalid_socket = INVALID_SOCKET;

        static constexpr int socket_error = SOCKET_ERROR;

        static void close(socket_handle handle);

        static void set_blocking(socket_handle handle, bool blocking);

        static std::string get_error();

        socket_impl() = delete;
    };
}

#endif