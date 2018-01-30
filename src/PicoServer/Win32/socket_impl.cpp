#include "Win32/socket_impl.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

namespace priv
{
    struct socket_initializer
    {
        socket_initializer();
        ~socket_initializer();
    };

    std::string get_error_message(DWORD code);

    const socket_initializer initializer{};
}

namespace ps
{
    void socket_impl::close(const socket_handle handle)
    {
        if (closesocket(handle) == socket_error)
            throw std::runtime_error("closesocket failed: " + get_error());
    }

    void socket_impl::set_blocking(const socket_handle handle, const bool blocking)
    {
        u_long block = blocking ? 0 : 1;
        if (ioctlsocket(handle, FIONBIO, &block) == socket_error)
            throw std::runtime_error("ioctlsocket failed: " + get_error());
    }

    std::string socket_impl::get_error()
    {
        const auto code = WSAGetLastError();
        return priv::get_error_message(code);
    }
}

namespace priv
{
    socket_initializer::socket_initializer()
    {
        WSADATA init;
        const auto result = WSAStartup(MAKEWORD(2, 2), &init);

        if (result != 0)
            throw std::runtime_error(("WSAStartup failed: " + get_error_message(result)).c_str());
    }

    socket_initializer::~socket_initializer()
    {
        if (WSACleanup() == ps::socket_impl::socket_error)
            std::cerr << "WSACleanup failed: " << ps::socket_impl::get_error() << std::endl;
    }

    std::string get_error_message(const DWORD code)
    {
        LPSTR buffer;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, code, 0,
                      reinterpret_cast<LPSTR>(&buffer), 0, nullptr);

        std::string error(buffer);

        LocalFree(buffer);

        return error;
    }
}
