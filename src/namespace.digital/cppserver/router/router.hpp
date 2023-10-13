#ifndef NAMESPACE_DIGITAL_ROUTER_HPP
#define NAMESPACE_DIGITAL_ROUTER_HPP
#include <iostream>

namespace namespacedigital {
  namespace cppserver {
    namespace router {
      class Router {
      public:
        Router() {
          //do nothing
        }
        std::string getHello();
      };
    }
  }

}
#endif
