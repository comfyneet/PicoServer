#include <iostream>
#include <sstream>
#include <PicoServer/pico_server.hpp>

int main()
{
    try
    {
        ps::server server(80, 10);

        server.map_get_route("^/get/([0-9]+)$", [](ps::context& context)
        {
            std::stringstream stream;
            stream << "Content-Type: text/html\r\n\r\n";
            stream << "<html><body>";
            stream << *context.get_request().get_uri_match(0);
            stream << "</body></html>";
            context.get_response().set_content(stream.str());
        });

        server.start();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
