//
// Created by Des Caldnd on 3/27/2024.
//

#ifndef MP_OS_SERVER_H
#define MP_OS_SERVER_H

#include <crow.h>
#include <unordered_map>
#include <logger.h>
//#include <mutex>
#include <shared_mutex>

class server
{
    crow::SimpleApp app;

    std::unordered_map<int, std::unordered_map<logger::severity, std::pair<std::string, bool>>> _streams;

    std::shared_mutex _mut; // разделяемый мьютекс, использование через std::shared_lock позволяет
    // нескольким потокам читать одновременно
    // std::lock_guard позволяет писать только одному потоку
public:

    explicit server(uint16_t port = 8080);

    server(const server&) = delete;
    server& operator=(const server&) = delete;
    server(server&&) noexcept = delete;
    server& operator=(server&&) noexcept = delete;
    ~server() noexcept = default;
};


#endif //MP_OS_SERVER_H
