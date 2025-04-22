#include <string>
#include <sstream>
#include <algorithm>
#include <utility>
#include "../include/client_logger.h"
#include <not_implemented.h>

std::unordered_map<std::string, std::pair<size_t, std::ofstream>> client_logger::refcounted_stream::_global_streams;


logger& client_logger::log(const std::string &text, logger::severity severity) & {
    for (const auto& [sev, streams] : _output_streams) {
        if (severity == sev) {
            std::string formatted_message = make_format(text, severity);

            // Записываем в файлы
            for (const auto& stream : streams.first) {
                *stream._stream.second << formatted_message << std::endl;
            }

            // Если second, дублируем в консоль
            if (streams.second) {
                std::cout << formatted_message << std::endl;
            }
        }
    }
    return *this;
}


std::string client_logger::make_format(const std::string &message, severity sev) const
{
    std::string formatted_message = _format;
    // меняем флаги на значения
    for (size_t i = 0; i < formatted_message.size(); ++i)
    {
        if (formatted_message[i] == '%')
        {
            if (i + 1 < formatted_message.size())
            {
                switch (formatted_message[i + 1])
                {
                    case 'd': // Дата
                        formatted_message.replace(i, 2, current_date_to_string());
                        break;
                    case 't': // Время
                        formatted_message.replace(i, 2, current_time_to_string());
                        break;
                    case 's': // Уровень серьезности
                        formatted_message.replace(i, 2, severity_to_string(sev));
                        break;
                    case 'm': // Сообщение
                        formatted_message.replace(i, 2, message);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return formatted_message;

}

client_logger::client_logger(const std::unordered_map<severity, std::pair<std::forward_list<refcounted_stream>, bool>>& streams, std::string format)
        : _output_streams(streams), _format(std::move(format)) {
    for (auto& [sev, stream_data] : _output_streams) {
        for (auto& stream : stream_data.first) {
            stream.open();
        }
    }

}

client_logger::flag client_logger::char_to_flag(char c) noexcept
{
    // символ -> флаг
    switch (c)
    {
        case 'd': return flag::DATE;
        case 't': return flag::TIME;
        case 's': return flag::SEVERITY;
        case 'm': return flag::MESSAGE;
        default:  return flag::NO_FLAG;
    }
}
client_logger::client_logger(const client_logger &other)
        : _output_streams(other._output_streams), _format(other._format) {}

client_logger& client_logger::operator=(const client_logger &other) {
    if (this != &other) {
        _output_streams = other._output_streams;
        _format = other._format;
    }
    return *this;
}

client_logger::client_logger(client_logger &&other) noexcept
        : _output_streams(std::move(other._output_streams)), _format(std::move(other._format)) {}



client_logger& client_logger::operator=(client_logger &&other) noexcept {
    if (this != &other) {
        _output_streams = std::move(other._output_streams);
        _format = std::move(other._format);
    }
    return *this;
}

client_logger::~client_logger() noexcept = default;


client_logger::refcounted_stream::refcounted_stream(const std::string &path) {
    auto it = _global_streams.find(path); // ищем путь в глобал мапе
    if (it != _global_streams.end()) {
        _stream.first = path;
        _stream.second = &it->second.second;
        it->second.first++;
    } else {
        _global_streams[path] = {1, std::ofstream(path)}; // создаем поток
        _stream.first = path;
        _stream.second = &_global_streams[path].second;
    }
}

client_logger::refcounted_stream::refcounted_stream(const refcounted_stream &oth)
        : _stream(oth._stream) {
    if (_stream.second) {
        _global_streams[_stream.first].first++;
    }
}

client_logger::refcounted_stream &
client_logger::refcounted_stream::operator=(const client_logger::refcounted_stream &oth)
{
    if (this != &oth) {
        if (_stream.second && --_global_streams[_stream.first].first == 0) {
            _global_streams.erase(_stream.first);
        }
        _stream = oth._stream;
        if (_stream.second) {
            _global_streams[_stream.first].first++;
        }
    }
    return *this;
}

client_logger::refcounted_stream::refcounted_stream(refcounted_stream &&oth) noexcept
        : _stream(std::move(oth._stream)) {
    oth._stream.second = nullptr;
}

client_logger::refcounted_stream& client_logger::refcounted_stream::operator=(refcounted_stream &&oth) noexcept {
    if (this != &oth) {
        if (_stream.second && --_global_streams[_stream.first].first == 0) {
            _global_streams.erase(_stream.first);
        }
        _stream = std::move(oth._stream);
        oth._stream.second = nullptr;
    }
    return *this;
}

client_logger::refcounted_stream::~refcounted_stream() {
    if (_stream.second && --_global_streams[_stream.first].first == 0) {
        _global_streams.erase(_stream.first);
    }
}


void client_logger::refcounted_stream::open() {
}

const std::string& client_logger::refcounted_stream::get_path() const {
    return _stream.first;
}