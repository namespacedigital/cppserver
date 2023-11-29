#ifndef NAMESPACE_DIGITAL_WS_CONTEXT_HPP
#define NAMESPACE_DIGITAL_WS_CONTEXT_HPP

#include <boost/asio.hpp>
#include "attributes.hpp"

#include <string>
#include <memory>

namespace namespacedigital {
    namespace cppserver {
        namespace router {
            class WsWorker;
            using endpoint = boost::asio::ip::tcp::endpoint;

            // --------------------------------------------------------------------------
            struct WsContext {
                endpoint remote_endpoint;
                endpoint local_endpoint;
                std::string uuid;
                std::weak_ptr<WsWorker> wsWorker;

                std::string target;
                Attributes attributes;

                std::string route_path;
            };

        }
    }
}

#endif