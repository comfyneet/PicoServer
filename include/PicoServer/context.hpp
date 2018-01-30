#ifndef PS_CONTEXT_HPP
#define PS_CONTEXT_HPP

#include <memory>
#include "PicoServer/config.hpp"
#include "PicoServer/non_copyable.hpp"

namespace ps
{
    class request;
    class response;

    class PS_API context : public virtual non_copyable
    {
    public:
        virtual ~context() = default;

        const request& get_request() const { return *request_; }

        const response& get_response() const { return *response_; }

    private:
        std::unique_ptr<request> request_;
        std::unique_ptr<response> response_;
    };
}

#endif