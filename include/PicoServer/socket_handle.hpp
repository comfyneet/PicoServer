#ifndef PS_SOCKET_HANDLE_HPP
#define PS_SOCKET_HANDLE_HPP

#include "PicoServer/config.hpp"

#ifdef PS_WINDOWS
#include <WinSock2.h>
typedef SOCKET socket_handle;
#else
typedef int socket_handle;
#endif

#endif
