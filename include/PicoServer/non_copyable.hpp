#ifndef PS_NON_COPYABLE_HPP
#define PS_NON_COPYABLE_HPP

#include "PicoServer/config.hpp"

namespace ps
{
    class PS_API non_copyable
    {
    public:
        non_copyable() = default;

        virtual ~non_copyable() = default;

        non_copyable(const non_copyable&) = delete;

        non_copyable& operator =(const non_copyable&) = delete;

        non_copyable(non_copyable&&) = delete;

        non_copyable& operator=(non_copyable&&) = delete;
    };
}

#endif
