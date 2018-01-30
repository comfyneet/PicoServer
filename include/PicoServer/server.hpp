#ifndef PS_SERVER_HPP
#define PS_SERVER_HPP

#include <functional>
#include <map>
#include <string>
#include "PicoServer/config.hpp"
#include "PicoServer/non_copyable.hpp"
#include "PicoServer/socket_handle.hpp"

namespace ps
{
    class request;
    class response;
    class session;

    class PS_API server : public non_copyable
    {
    public:
        server(uint16_t port, uint32_t backlog);

        virtual ~server();

        void add_default_route(const std::function<void(const request&, const response&)>& action);

        void add_get_route(const std::string& route_name, const std::function<void(const request&, const response&)>& action);

        void start();

        void stop();

    private:
        void run(socket_handle socket, const std::string& ip) const;

        std::function<void(const request&, const response&)> default_route_;

        std::map<std::string, std::function<void(const request&, const response&)>> get_routes_;

        //std::map<std::string, std::function<void(const request&, const response&)>> post_routes_;

        uint16_t port_;

        socket_handle listener_;

        bool running_;
    };
}

#endif
