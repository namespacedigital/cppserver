#ifndef namespacedigital_server_hpp
#define namespacedigital_server_hpp

#include <boost/asio.hpp>
#include <boost/beast/http.hpp>
#include <string>
#include <vector>
#include <list>
#include "./router/router.hpp"
#include "./configuration/driver/ini.hpp"

namespace namespacedigital {
  namespace cppserver {

    namespace beast = boost::beast;         // from <boost/beast.hpp>
    namespace http = beast::http;           // from <boost/beast/http.hpp>
    namespace net = boost::asio;            // from <boost/asio.hpp>
    using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
    using endpoint = boost::asio::ip::tcp::endpoint;
    using namespacedigital::cppserver::router::route_cb;

    class Server {
    public:
      class ServerRoute {
      public:
        ServerRoute(Server& s, std::string path) : _server(s), _path(std::move(path)) {
          //do nothing
        }

        ServerRoute& get(route_cb&& cb) { _server.get(_path, std::move(cb)); return *this; };

        ServerRoute& put(route_cb&& cb) { _server.put(_path, std::move(cb)); return *this; };

        ServerRoute& post(route_cb&& cb) { _server.post(_path, std::move(cb)); return *this; };

        ServerRoute& options(route_cb&& cb) { _server.options(_path, std::move(cb)); return *this; };

        ServerRoute& del(route_cb&& cb) { _server.del(_path, std::move(cb)); return *this; };

      private:
        Server& _server;
        std::string _path;
      };

    public:
      Server();
      ~Server();

      Server(const Server&) = delete;
      Server& operator=(const Server&) = delete;

      Server(Server&&) = delete;
      Server& operator=(Server&&) = delete;
      ServerRoute add_route(const std::string& path) { return { *this, path }; }

      // Legacy API, should not be used anymore to avoid PATH duplication
      Server& get(const std::string& path, route_cb&& cb);
      Server& put(const std::string& path, route_cb&& cb);
      Server& post(const std::string& path, route_cb&& cb);
      Server& options(const std::string& path, route_cb&& cb);
      Server& del(const std::string& path, route_cb&& cb);

      void listen(unsigned short port = 8080, const std::string& address = "0.0.0.0");
      void stop();
      void run();
      void wait();

      const endpoint& getEndpoint() const { return _endpoint; }
      int port() const { return endpoint().port(); }

    private:
      int _worker_pool{ 1 };
      endpoint _endpoint;
      std::string _doc_root;
    };
  }
}

#endif
