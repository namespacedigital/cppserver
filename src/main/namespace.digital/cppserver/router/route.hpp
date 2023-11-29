#ifndef NAMESPACE_DIGITAL_ROUTE_HPP
#define NAMESPACE_DIGITAL_ROUTE_HPP

#include "request.hpp"
#include "response.hpp"
#include "ws_handler.hpp"
#include "ws_context.hpp"


namespace namespacedigital {
  namespace cppserver {
    namespace router {
      using route_cb = std::function<void(const Request& req, Response& res)>;


      class Route {
      public:
        explicit Route(const std::string& path, route_cb&& cb = [](const auto& req, auto& res) {});
        Route(const std::string& path, WsHandler&& handler);

        bool match(Request& req) const noexcept;

        void execute(const Request& req, Response& res) const {
          _cb(req, res);
        }


        // Websocket
        void connect(const WsContext& ctx) const {
          _ws_handler.on_connect(ctx);
        }
        void receive(const WsContext& ctx, const char* data, std::size_t size, bool is_text) const {
          _ws_handler.on_receive(ctx, data, size, is_text);
        }
        void disconnect(const WsContext& ctx) const {
          _ws_handler.on_disconnect(ctx);
        }

        [[nodiscard]] const std::string& path() const noexcept { return _path; }
        [[nodiscard]] const std::vector<std::string>& segments() const noexcept { return _segments; }


      private:
        std::string _path;
        std::vector<std::string> _segments;
        WsHandler  _ws_handler;
        route_cb    _cb;
      };
    }
  }
}
#endif
