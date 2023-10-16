#include <namespace.digital/cppserver/configuration/driver/ini.hpp>
#include <spdlog/spdlog.h>

int main(int argc, char* argv[]) {
    auto mainLogger = spdlog::default_logger();
    mainLogger->set_pattern("[source %s] [function %!] [line %#] %v");
    if (argc != 2) {
        mainLogger->error("Usage: streamplatform <config_file> absolute path\n");
        return EXIT_FAILURE;
    }

    std::string configFile = argv[1];
    auto& iniConfig = namespacedigital::cppserver::configuration::Ini::getInstance();
    iniConfig.init(configFile);

    mainLogger->info("Server host: {}", iniConfig.getServerHost());
    mainLogger->info("Server port: {}", iniConfig.getServerPort());
    mainLogger->info("Doc root: {}", iniConfig.getDocRoot());

}

// https://github.com/gabime/spdlog/issues/318
// #include <spdlog/sinks/basic_file_sink.h>
// #include <spdlog/sinks/stdout_color_sinks.h>
// #include <spdlog/spdlog.h>

// int main(int, char**) {

//     // spdlog::set_pattern("[source %s] [function %!] [line %#] %v");
//     // spdlog::set_level(spdlog::level::trace);

//     // SPDLOG_TRACE("This trace");
//     // SPDLOG_DEBUG("This is debug..");
//     // SPDLOG_INFO("This is info..");
//     // SPDLOG_ERROR("This is error..");
//     // SPDLOG_CRITICAL("This is critical..");

//     auto console_logger = spdlog::stdout_color_mt("console");
//     console_logger->set_pattern("[source %s] [function %!] [line %#] %v");
//     SPDLOG_LOGGER_ERROR(console_logger, "This is error..");
//     return 0;
// }