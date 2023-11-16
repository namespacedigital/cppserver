
#ifndef NAMESPACE_DIGITAL_ROUTER_HPP
#define NAMESPACE_DIGITAL_ROUTER_HPP
#include <boost/beast/http.hpp>
#include <iostream>
#include <spdlog/spdlog.h>
#include "route.hpp"

namespace namespacedigital {
    namespace cppserver {
        namespace router {
            namespace beast = boost::beast;
            class Router {
            public:
                std::string getHello();
                Router(const Router& obj) = delete;

                static Router* getInstance() {
                    if (instancePtr == NULL) {
                        instancePtr = new Router();
                        return instancePtr;
                    } else {
                        return instancePtr;
                    }
                }
                using routes = std::unordered_map<beast::http::verb, std::vector<Route>>;

                void add_route(beast::http::verb v, Route&& r);
                routes::const_iterator find(beast::http::verb v) const noexcept {
                    return _routes.find(v);
                }
                routes::const_iterator begin() const noexcept { return _routes.begin(); }
                routes::const_iterator end() const noexcept { return _routes.end(); }
                routes      _routes;
            private:
                std::shared_ptr<spdlog::logger> routerLogger = spdlog::default_logger();
                static Router* instancePtr;
                Router() {}
            };
        }
    }
}
#endif
