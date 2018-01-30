#include "PicoServer/session.hpp"
#include "PicoServer/request.hpp"

namespace ps
{
    session::session(std::unique_ptr<request>& request)
    {
        request_ = std::move(request);
    }

    request& session::get_request() const
    {
        return *request_;
    }
}
