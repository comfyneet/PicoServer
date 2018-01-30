#ifndef PS_SOCKET_IMPL_HPP
#define PS_SOCKET_IMPL_HPP

#include "PicoServer/config.hpp"

#ifdef PS_WINDOWS
#  include "Win32/socket_impl.hpp"
#else
#  error Linux
#endif

#endif