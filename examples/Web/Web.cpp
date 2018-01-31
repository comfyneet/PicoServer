#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <PicoServer/pico_server.hpp>

int main()
{
    try
    {
        ps::server server(80, 10);

        server.map_post_route("^/upload$", [](const ps::request& request)
        {
            if (request.get_body())
            {
                const auto request_body = *request.get_body();

                std::map<std::string, std::string> headers;
                headers["Content-Type"] = "application/octet-stream";
                headers["Content-Length"] = std::to_string(request_body.size());

                const auto response_body = std::string(request_body.begin(), request_body.end());

                return ps::response(ps::status_code::ok, headers, response_body);
            }
            else return ps::response(ps::status_code::bad_request, {}, {});
        });

        server.map_get_route("^/request-info", [](const ps::request& request)
        {
            std::stringstream body_stream;
            body_stream << "<!DOCTYPE html><html><body>";
            body_stream << "<h1>" << request.get_remote_ip() << ":" << std::to_string(request.get_remote_port()) <<
                "</h1>";
            body_stream << "<h2>"
                << request.get_method() << " "
                << request.get_uri() << " "
                << request.get_http_version() << "</h2>";
            for (const auto& header : request.get_headers())
                body_stream << "<p>" << header.first << ": " << header.second << "</p>";
            body_stream << "<hr>";
            if (request.get_body())
                body_stream << std::string((*request.get_body()).begin(), (*request.get_body()).end());
            body_stream << "</body></html>";
            const auto body = body_stream.str();

            std::map<std::string, std::string> headers;
            headers["Content-Type"] = "text/html";
            headers["Content-Length"] = std::to_string(body.length());

            return ps::response(ps::status_code::ok, headers, body);
        });

        server.map_get_route("^/regex/([0-9]+)$", [](const ps::request& request)
        {
            const auto body = *request.get_uri_match(1);

            std::map<std::string, std::string> headers;
            headers["Content-Type"] = "text/plain";
            headers["Content-Length"] = std::to_string(body.length());

            return ps::response(ps::status_code::ok, headers, body);
        });

        server.map_default_route([](const ps::request& request)
        {
            const std::experimental::filesystem::path root_path = "wwwroot";
            auto path = root_path;
            if (request.get_uri() == "/") path /= "/index.html";
            else
            {
                // TODO
                path /= request.get_uri();
            }

            if (std::experimental::filesystem::exists(path))
            {
                const std::ifstream file_stream(path);
                std::stringstream body_stream;
                body_stream << file_stream.rdbuf();
                const auto body = body_stream.str();

                std::map<std::string, std::string> headers;
                headers["Content-Length"] = path.extension() == ".html" ? "text/html" : "text/plain";
                headers["Content-Length"] = std::to_string(body.length());

                return ps::response(ps::status_code::ok, headers, body);
            }
            else
            {
                std::stringstream body_stream;
                body_stream << "<!DOCTYPE html><html><body>";
                body_stream << "<h1>File \"" << std::experimental::filesystem::canonical(path) << "\" not found.</h1>";
                body_stream << "</body></html>";
                const auto body = body_stream.str();

                std::map<std::string, std::string> headers;
                headers["Content-Type"] = "text/html";
                headers["Content-Length"] = std::to_string(body.length());

                return ps::response(ps::status_code::not_found, headers, body);
            }
        });

        server.start();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
