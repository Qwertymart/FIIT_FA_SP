#include <not_implemented.h>
// #include <httplib.h>
#include "../include/server_logger.h"

#ifdef _WIN32
#include <process.h>
#elif
#include <unistd.h>
#endif

server_logger::~server_logger() noexcept
{
    httplib::Client client("http://localhost:8080");
    httplib::Params params;
    params.emplace("pid", std::to_string(inner_getpid()));

    client.Get("/destroy", params, httplib::Headers());
}



logger& server_logger::log(const std::string &text, logger::severity severity) & {
    httplib::Params params;
    params.emplace("pid", std::to_string(inner_getpid()));
    params.emplace("sev", severity_to_string(severity));
    params.emplace("message", text);
    httplib::Client client("http://localhost:8080");

    client.Get("/log", params, httplib::Headers());

    return *this;
}




server_logger::server_logger(const std::string& dest,
                             const std::unordered_map<logger::severity, std::pair<std::string, bool>> &streams)
    :_client(dest.c_str())
{
    httplib::Client client("http://localhost:8080");
    for (const auto &[fst, snd] : streams)
    {
        httplib::Params par;
        par.emplace("pid", std::to_string(server_logger::inner_getpid()));
        par.emplace("sev", severity_to_string(fst));
        par.emplace("path", snd.first);
        par.emplace("console", snd.second ? "1" : "0");

        auto res = client.Get("/init", par, httplib::Headers());
    }

}

int server_logger::inner_getpid()
{
#ifdef _WIN32
    return ::_getpid();
#else
    return getpid();
#endif
}

server_logger::server_logger(server_logger const& other)
        : _client(other._client.host(), other._client.port()) // Копируем параметры клиента из другого объекта
{
}

server_logger &server_logger::operator=(const server_logger &other)
{
    if (this != &other) {
        _client = httplib::Client(other._client.host(), other._client.port());
    }
    return *this;
}

server_logger::server_logger(server_logger &&other) noexcept
        : _client(std::move(other._client))
{
}

server_logger &server_logger::operator=(server_logger &&other) noexcept
{
    if (this != &other) {
        _client = std::move(other._client);
    }
    return *this;
}
