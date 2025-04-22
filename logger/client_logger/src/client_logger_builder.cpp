#include <filesystem>
#include <utility>
#include <not_implemented.h>
#include "../include/client_logger_builder.h"
#include <not_implemented.h>

using namespace nlohmann;

logger_builder& client_logger_builder::add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) &
{
    auto& streams = _output_streams[severity].first;

    // Проверяем, существует ли уже такой путь в списке потоков
    bool exists = false;
    for (const auto& stream : _output_streams[severity].first) {
        if (stream.get_path() == stream_file_path) {
            exists = true;
            break;
        }
    }


    if (!exists) {
        streams.emplace_front(stream_file_path);
    }
    return *this;
}


logger_builder& client_logger_builder::add_console_stream(
    logger::severity severity) &
{
   _output_streams[severity].second = true;
    return *this;
}

logger_builder& client_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path) &
{
    std::ifstream file(configuration_file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open configuration file: " + configuration_file_path);
    }

    json config;
    file >> config;
    file.close();

    auto settings = config.at(configuration_path);

    if (settings.contains("format")) {
        _format = settings["format"];
    }

    if (settings.contains("streams")) {
        for (const auto& stream : settings["streams"]) {
            std::string type = stream["type"];
            auto severity = static_cast<logger::severity>(stream["severity"]);

            if (type == "console") {
                add_console_stream(severity);
            } else if (type == "file") {
                add_file_stream(stream["path"], severity);
            }
        }
    }

    return *this;
}

logger_builder& client_logger_builder::clear() &
{
    _output_streams.clear();
    _format.clear();
    return *this;
}

logger *client_logger_builder::build() const
{
    return new client_logger(_output_streams, _format);
}

logger_builder& client_logger_builder::set_format(const std::string &format) &
{
    _format = format;
    return *this;
}

void client_logger_builder::parse_severity(logger::severity sev, nlohmann::json& j)
{
    throw not_implemented("void client_logger_builder::parse_severity(logger::severity, nlohmann::json&)", "your code should be here...");
}

logger_builder& client_logger_builder::set_destination(const std::string &format) &
{
    throw not_implemented("logger_builder *client_logger_builder::set_destination(const std::string &format)", "invalid call");
}
