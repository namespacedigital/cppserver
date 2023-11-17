#pragma once

#include "request.hpp"
#include "response.hpp"
#include "ws_context.hpp"

#include <functional>

namespace namespacedigital {
    namespace cppserver {
        namespace router {
            // --------------------------------------------------------------------------
            // Websocket callbacks
            // --------------------------------------------------------------------------
            using ws_on_connect_cb = std::function<void(const WsContext&)>;
            using ws_on_receive_cb = std::function<void(const WsContext&, const char*, std::size_t, bool)>;
            using ws_on_disconnect_cb = std::function<void(const WsContext& ctx)>;
            using ws_on_error_cb = std::function<void(boost::system::error_code, const char* what)>;

            // --------------------------------------------------------------------------
            struct WsHandler {
                ws_on_connect_cb on_connect = [](const WsContext&) {};
                ws_on_receive_cb on_receive = [](const WsContext&, const char*, std::size_t, bool) {};
                ws_on_disconnect_cb on_disconnect = [](const WsContext& ctx) {};
                ws_on_error_cb on_error = [](boost::system::error_code, const char* what) {};
            };

        }
    }
}
