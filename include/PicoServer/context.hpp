#ifndef PS_CONTEXT_HPP
#define PS_CONTEXT_HPP

#include "PicoServer/config.hpp"
#include "PicoServer/non_copyable.hpp"
#include "PicoServer/request.hpp"
#include "PicoServer/response.hpp"

namespace ps
{
    class PS_API context : public virtual non_copyable
    {
    public:
        context(const request& request, const response& response) :
            request_{request},
            response_{response}
        {
        }

        virtual ~context() = default;

        const request& get_request() const { return request_; }

        response& get_response() { return response_; }

    private:
        request request_;
        response response_;
    };
}

#endif
