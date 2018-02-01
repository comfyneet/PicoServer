#include "Unix/socket_impl.hpp"
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

namespace ps
{
    void socket_impl::close(const socket_handle handle)
    {
        if (::close(handle) == socket_error)
            throw std::runtime_error("close failed: " + get_error());
    }

    // https://stackoverflow.com/a/33088114/7821462
    void socket_impl::set_blocking(const socket_handle handle, const bool blocking)
    {
        const int flags = fcntl(handle, F_GETFL);
        if (flags == socket_error)
            throw std::runtime_error("fcntl failed: " + get_error());

        if ((flags & O_NONBLOCK) && !blocking) return;
        if (!(flags & O_NONBLOCK) && blocking) return;

        if (fcntl(handle, F_SETFL, blocking ? flags ^ O_NONBLOCK : flags | O_NONBLOCK) == socket_error)
            throw std::runtime_error("fcntl failed: " + get_error());
    }

    std::string socket_impl::get_error()
    {
        return std::string(strerror(errno));
    }
}
