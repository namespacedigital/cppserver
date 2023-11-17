#include <namespace.digital/cppserver/configuration/driver/ini.hpp>
#include <spdlog/spdlog.h>
#include <iostream>
#include <namespace.digital/cppserver/server.hpp>
#include <namespace.digital/cppserver/router/ws_handler.hpp>
#include <namespace.digital/cppserver/router/ws_context.hpp>
#include <namespace.digital/cppserver/router/ws_worker.hpp>
#include <boost/json/src.hpp>

#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <chrono>
#include <thread>


using namespace boost::json;
using namespace namespacedigital::cppserver::router;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
using namespacedigital::cppserver::router::Request;
using namespacedigital::cppserver::router::Response;

//------------------------------------------------------------------------------
using Sessions = std::unordered_map<std::string /* UUID */, std::weak_ptr<WsWorker>>;
using Rooms = std::unordered_map<std::string /* ROOM */, Sessions>;

Rooms rooms;


inline std::string exec(std::string command) {
    char buffer[128];
    std::string result = "";

    // Open pipe to file
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }

    // read till end of process:
    while (!feof(pipe)) {

        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);
    return result;
}

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

    WsHandler handler;
    handler.on_connect = [](const WsContext& ctx) {
        std::cout << "on connect: " << ctx.uuid << std::endl;
        std::cout << "    remote: " << ctx.remote_endpoint << std::endl;
        std::cout << "    target: " << ctx.target << std::endl;
        std::cout << "     route: " << ctx.route_path << std::endl;
        for (const auto& attr : ctx.attributes) {
            std::cout << " attribute: " << attr.first << " = " << attr.second.as_string() << std::endl;
        }
        rooms[ctx.attributes["room"].as_string()][ctx.uuid] = ctx.wsWorker;
        };
    handler.on_receive = [](const WsContext& ctx, const char* data, std::size_t size, bool is_text) {
        std::cout << "on receive: " << ctx.uuid << std::endl;

        for (auto& [uuid, session] : rooms[ctx.attributes["room"].as_string()]) {
            if (auto s = session.lock(); s) {
                s->send(std::string(data, size));
            }
        }
        };
    handler.on_disconnect = [](const WsContext& ctx) {
        std::cout << "on disconnect: " << ctx.uuid << std::endl;
        for (auto& [name, room] : rooms) {
            room.erase(ctx.uuid);
        }
        };

    namespacedigital::cppserver::Server server;

    server.ws("/chat/:room", std::move(handler));

    boost::asio::thread_pool tp(8);

    server.broadcast([&]() {
        std::cout << "Starting broadcasting server service: " << std::endl;


        boost::asio::post(tp, [] {
            while (1) {

                std::cout << "Broadcasting to all" << std::endl;
                for (auto& [roomName, sessions] : rooms) {
                    std::cout << roomName << std::endl;
                    for (auto& [uuid, session] : sessions) {
                        std::cout << "Room: " << uuid << std::endl;
                        if (auto s = session.lock(); s) {
                            s->send(std::string(exec("top -l 1 -n 0")));

                        }

                    }

                }
                std::this_thread::sleep_for(std::chrono::milliseconds(200));

            }});






        }).listen(8090, "0.0.0.0");

}
