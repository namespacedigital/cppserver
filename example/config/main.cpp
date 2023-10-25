#include <namespace.digital/cppserver/configuration/driver/ini.hpp>
#include <spdlog/spdlog.h>

int main(int argc, char* argv[]) {
    // spdlog::set_pattern("[source %s] [function %!] [line %#] %v");
    // spdlog::set_level(spdlog::level::trace);
    // SPDLOG_TRACE("This trace");
    // SPDLOG_DEBUG("This is debug..");
    // SPDLOG_INFO("This is info..");
    // SPDLOG_ERROR("This is error..");
    // SPDLOG_CRITICAL("This is critical..");
    if (argc != 2) {
        SPDLOG_ERROR("Usage: streamplatform <config_file> absolute path\n");
        return EXIT_FAILURE;
    }

    std::string configFile = argv[1];
    auto& iniConfig = namespacedigital::cppserver::configuration::Ini::getInstance();
    iniConfig.init(configFile);

    spdlog::info("Server host: {}", iniConfig.getServerHost());
    spdlog::info("Server port: {}", iniConfig.getServerPort());
    spdlog::info("Doc root: {}", iniConfig.getDocRoot());

}
