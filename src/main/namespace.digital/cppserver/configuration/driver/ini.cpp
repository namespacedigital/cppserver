#include "ini.hpp"
namespace {
  std::once_flag flag;
  std::unique_ptr<namespacedigital::cppserver::configuration::Ini> iniInstance;
}
namespace namespacedigital {
  namespace cppserver {
    namespace configuration {

      Ini::Ini() {
      }

      Ini::~Ini() {
      }

      Ini& Ini::getInstance() {
        std::call_once(flag, [] {
          iniInstance = std::make_unique<Ini>();
          });
        return *iniInstance;
      }

      void Ini::init(std::string configFile) {
        this->setConfigurationFile(configFile);
        boost::property_tree::ini_parser::read_ini(this->fileName, pt);
        this->setServerHost();
        this->setServerPort();
        this->setThreadsNumber();
        this->setWorkersNumber();
        this->setPoolType();
        this->setDocRoot();
      }

      std::string Ini::getConfig(std::string configName) {
        try {
          return pt.get<std::string>(configName);
        } catch (std::exception& e) {
          iniLogger->error("getConfig(): {}", e.what());
        }
        return "";
      }

      std::string Ini::getServerHost() {
        return this->serverHost;
      }


      size_t Ini::getServerPort() {
        return this->serverPort;
      }


      std::string Ini::getDocRoot() {
        return this->docRoot;
      }

      size_t& Ini::getThreadsNumber() {
        return this->threadsNumber;
      }

      size_t Ini::getWorkersNumber() {
        return this->workersNumber;
      }

      std::string Ini::getPoolType() {
        return this->poolType;
      }

      void Ini::setConfigurationFile(std::string fileName) {
        this->fileName = fileName;
      }

      void Ini::setServerHost() {
        try {
          std::string host = pt.get<std::string>("server.host");
          this->serverHost = host;
        } catch (std::exception& e) {
          iniLogger->error("setServerHost(): {}", e.what());
        }
      }

      void Ini::setServerPort() {
        try {
          this->serverPort = stoi(pt.get<std::string>("server.port"));
        } catch (std::exception& e) {
          iniLogger->error("setServerPort(): {}", e.what());
        }
      }

      void Ini::setThreadsNumber() {
        try {
          this->threadsNumber = stoi(pt.get<std::string>("server.threads_number"));
        } catch (std::exception& e) {
          iniLogger->error("setThreadsNumber(): {}", e.what());
        }
      }

      void Ini::setWorkersNumber() {
        try {
          this->workersNumber = stoi(pt.get<std::string>("server.workers_number"));
        } catch (std::exception& e) {
          iniLogger->error("setWorkersNumber(): {}", e.what());
        }
      }

      void Ini::setPoolType() {
        try {
          this->poolType = pt.get<std::string>("server.pool_type");
        } catch (std::exception& e) {
          iniLogger->error("setPoolType(): {}", e.what());
        }
      }

      void Ini::setDocRoot() {
        try {
          this->docRoot = pt.get<std::string>("server.doc_root");
        } catch (std::exception& e) {
          iniLogger->error("setDocRoot(): {}", e.what());
        }
      }
    }
  }
}