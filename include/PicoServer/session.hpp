#ifndef PS_SESSION_HPP
#define PS_SESSION_HPP

#include <memory>
#include "PicoServer/config.hpp"
#include "PicoServer/non_copyable.hpp"

namespace ps
{
    class request;

    class PS_API session : public non_copyable
    {
    public:
        explicit session(std::unique_ptr<request>& request);

        request& get_request() const;

    private:
        std::unique_ptr<request> request_;
    };
}

#endif