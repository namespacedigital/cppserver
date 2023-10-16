
#ifndef namespacedigital_configuration_hpp
#define namespacedigital_configuration_hpp
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <spdlog/spdlog.h>


namespace namespacedigital {
  namespace cppserver {
    namespace configuration {
      using boost::property_tree::ptree;
      class Ini {
      public:
        Ini();

        ~Ini();

        Ini(const Ini&) = delete;

        Ini& operator=(const Ini&) = delete;

        Ini(Ini&&) = delete;

        Ini& operator=(Ini&&) = delete;

        static Ini& getInstance();

        void init(std::string configFile);

        std::string getConfig(std::string configName);

        std::string getServerHost();

        size_t getServerPort();

        std::string getDocRoot();

        size_t& getThreadsNumber();

        size_t getWorkersNumber();

        std::string getPoolType();
      private:
        boost::property_tree::ptree pt;
        std::shared_ptr<spdlog::logger> iniLogger = spdlog::default_logger();
        std::string fileName;
        std::string serverHost;
        size_t serverPort;
        std::string docRoot;
        size_t threadsNumber;
        size_t workersNumber;
        std::string poolType;

        void setConfigurationFile(std::string fileName);

        void setServerHost();

        void setServerPort();

        void setThreadsNumber();

        void setWorkersNumber();

        void setPoolType();

        void setDocRoot();
      };
    }
  }
}

#endif
