#ifndef PS_UNIX_SOCKET_IMPL_HPP
#define PS_UNIX_SOCKET_IMPL_HPP

#include <string>
#include "PicoServer/socket_handle.hpp"

namespace ps
{
    class socket_impl
    {
    public:
        static constexpr socket_handle invalid_socket = -1;

        static constexpr int socket_error = -1;

        static void close(socket_handle handle);

        static void set_blocking(socket_handle handle, bool blocking);

        static std::string get_error();

        socket_impl() = delete;
    };
}

#endif
