#ifndef PS_SERVER_HPP
#define PS_SERVER_HPP

#include <functional>
#include <map>
#include <optional>
#include <string>
#include "PicoServer/config.hpp"
#include "PicoServer/non_copyable.hpp"
#include "PicoServer/socket_handle.hpp"

namespace ps
{
    class request;
    class response;

    class PS_API server : public virtual non_copyable
    {
    public:
        server(uint16_t port, uint32_t backlog);

        virtual ~server();

        void map_default_route(const std::function<response(const request&)>& func);

        void map_get_route(const std::string& template_name, const std::function<response(const request&)>& func);

        void map_post_route(const std::string& template_name, const std::function<response(const request&)>& func);

        void start();

        void stop();

    private:
        void run(socket_handle socket, const std::string& ip, uint16_t port) const;

        std::optional<std::function<response(const request&)>> default_route_;

        std::map<std::string, std::function<response(const request&)>> get_routes_;

        std::map<std::string, std::function<response(const request&)>> post_routes_;

        uint16_t port_;

        socket_handle listener_;

        bool running_;
    };
}

#endif
