#ifndef PS_SESSION_HPP
#define PS_SESSION_HPP

#include "PicoServer/config.hpp"
#include "PicoServer/non_copyable.hpp"

namespace ps
{
    class PS_API session : public virtual non_copyable
    {
    public:
        virtual ~session() = default;
    };
}

#endif
