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
    class context;

    class PS_API server : public virtual non_copyable
    {
    public:
        server(uint16_t port, uint32_t backlog);

        virtual ~server();

        void add_default_route(const std::function<void(context&)>& func);

        void map_get_route(const std::string& template_name, const std::function<void(context&)>& func);

        void start();

        void stop();

    private:
        void run(socket_handle socket, const std::string& ip) const;

        std::function<void(context&)> default_route_;

        std::map<std::string, std::function<void(context&)>> get_routes_;

        //std::map<std::string, std::function<void(context&)>> post_routes_;

        uint16_t port_;

        socket_handle listener_;

        bool running_;
    };
}

#endif
