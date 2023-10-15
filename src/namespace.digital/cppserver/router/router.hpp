#ifndef NAMESPACE_DIGITAL_ROUTER_HPP
#define NAMESPACE_DIGITAL_ROUTER_HPP
#include <iostream>
#include <spdlog/spdlog.h>

namespace namespacedigital {
  namespace cppserver {
    namespace router {
      class Router {
      public:
        Router() {
          //do nothing
        }
        std::string getHello();
      private:
        std::shared_ptr<spdlog::logger> routerLogger = spdlog::default_logger();
      };
    }
  }

}
#endif
