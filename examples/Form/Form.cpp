#include <iostream>
#include <thread>
#include <PicoServer/pico_server.hpp>

int main()
{
    try
    {
        ps::server server(80, 10);

        server.map_get_route("^/info$", [](const ps::context&)
        {
            //response.append("a");
        });

        server.start();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
