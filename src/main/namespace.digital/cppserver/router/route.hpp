#ifndef NAMESPACE_DIGITAL_ROUTE_HPP
#define NAMESPACE_DIGITAL_ROUTE_HPP

#include "request.hpp"
#include "response.hpp"


namespace namespacedigital {
  namespace cppserver {
    namespace router {
      using route_cb = std::function<void(const Request& req, Response& res)>;


      class Route {
      public:
        explicit Route(const std::string& path, route_cb&& cb = [](const auto& req, auto& res) {});

        bool match(Request& req) const noexcept;

        void execute(const Request& req, Response& res) const {
          _cb(req, res);
        }


        [[nodiscard]] const std::string& path() const noexcept { return _path; }
        [[nodiscard]] const std::vector<std::string>& segments() const noexcept { return _segments; }


      private:
        std::string _path;
        std::vector<std::string> _segments;

        route_cb    _cb;
      };
    }
  }
}
#endif
