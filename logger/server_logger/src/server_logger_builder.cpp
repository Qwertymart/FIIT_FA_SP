#include <not_implemented.h>
#include <fstream>
#include "../include/server_logger_builder.h"

logger_builder& server_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity) &
{
    _output_streams[severity].first = stream_file_path;
    _output_streams[severity].second = false;
    return *this;
}

logger_builder& server_logger_builder::add_console_stream(
    logger::severity severity) &
{
    _output_streams[severity].second = true;
    return *this;
}

logger_builder& server_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path) &
{
    std::ifstream config_file(configuration_file_path);
    if (!config_file.is_open()) {
        throw std::runtime_error("Cannot open configuration file: " + configuration_file_path);
    }

    try {
        nlohmann::json config_json;
        config_file >> config_json;

        if (config_json.contains(configuration_path)) {
            auto& config = config_json[configuration_path];

            if (config.contains("destination")) {
                _destination = config["destination"].get<std::string>();
            }

            if (config.contains("streams")) {
                _output_streams.clear();
                for (auto& stream : config["streams"]) {
                    std::string type = stream["type"].get<std::string>();
                    auto sev = static_cast<logger::severity>(stream["severity"].get<int>());

                    if (type == "console") {
                        _output_streams[sev] = {"", true};
                    } else if (type == "file") {
                        std::string path = stream["path"].get<std::string>();
                        _output_streams[sev] = {path, false};
                    }
                }
            }
        }

    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing configuration: " + std::string(e.what()));
    }

    return *this;
}

logger_builder& server_logger_builder::clear() &
{
    _output_streams.clear();
    _destination.clear();
    return *this;
}

logger *server_logger_builder::build() const
{
    return new server_logger(_destination, _output_streams);
}

logger_builder& server_logger_builder::set_destination(const std::string& dest) &
{
    _destination = dest;
    return *this;
}

logger_builder& server_logger_builder::set_format(const std::string &format) &
{
    throw not_implemented("logger_builder& server_logger_builder::set_format(const std::string &) &", "your code should be here...");
}
