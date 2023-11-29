#include <namespace.digital/cppserver/configuration/driver/ini.hpp>
#include <spdlog/spdlog.h>
#include <iostream>
#include <namespace.digital/cppserver/server.hpp>
#include <boost/json/src.hpp>

using namespace boost::json;
using namespacedigital::cppserver::router::Request;
using namespacedigital::cppserver::router::Response;

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>

int main(int argc, char* argv[]) {

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

    namespacedigital::cppserver::Server server;

    server.get("/api/v1", [](const Request& req, Response& res) {
        res.result(http::status::ok);
        res.keep_alive(false);
        res.set(http::field::content_type, "application/json");

        object obj;                                                     // construct an empty object
        obj["pi"] = 3.141;                                            // insert a double
        obj["happy"] = true;                                          // insert a bool
        obj["name"] = "Boost";                                        // insert a string
        obj["nothing"] = nullptr;                                     // insert a null
        obj["answer"].emplace_object()["everything"] = 42;            // insert an object with 1 element
        obj["list"] = { 1, 0, 2 };                                    // insert an array with 3 elements
        obj["object"] = { {"currency", "USD"}, {"value", 42.99} };

        res.body() = serialize(obj);
        }).listen(8090, "0.0.0.0");
}
