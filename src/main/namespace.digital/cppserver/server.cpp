#include "server.hpp"
#include "./configuration/driver/ini.hpp"


using namespacedigital::cppserver::Server;
using configuration = namespacedigital::cppserver::configuration::Ini;
using namespacedigital::cppserver::router::Router;
using namespacedigital::cppserver::router::WsHandler;
using namespacedigital::cppserver::router::route_cb;
using namespacedigital::cppserver::router::Route;
using namespacedigital::cppserver::router::HttpWorker;


Server::Server() {
  //do nothing
}

Server::~Server() {
  stop();
}

void Server::listen(unsigned short port, const std::string& address) {

  auto& configuration = configuration::Ini::getInstance();

  std::string serverHost = address;
  unsigned short serverPort = port;

  if (configuration.getServerHost().size()) {
    serverHost = configuration.getServerHost();
  }

  if (configuration.getServerPort()) {
    serverPort = configuration.getServerPort();
  }

  if (configuration.getDocRoot().size()) {
    _doc_root = configuration.getDocRoot();
  }

  _worker_pool = configuration.getWorkersNumber();

  // Run the I/O service on the requested number of threads
  size_t const threads_count = configuration.getThreadsNumber();
  //    std::list<HttpWorker> workers;
  net::io_context ioc{ (int)threads_count };

  const auto ipAddress = net::ip::make_address(serverHost);
  tcp::acceptor acceptor{ ioc, {ipAddress, serverPort} };

  // keep the worker active (feed the dog) instead of work guard for threads
  std::make_shared<HttpWorker>(acceptor, _doc_root, ioc)->start();
  for (int i = 0; i < _worker_pool; ++i) {
    std::make_shared<HttpWorker>(acceptor, _doc_root, ioc)->start();
    // std::cout << "worker = " << i << std::endl;
  }

  std::vector<std::thread> v;
  v.reserve(threads_count - 1);
  for (size_t i = 0; i < threads_count - 1; ++i) { // add thraed_count threads into the pool
    v.emplace_back([&ioc] { ioc.run(); });
    // std::cout << "thread = " << i << std::endl;
  }

  ioc.run();
}

void Server::stop() {
  //do nothint
}

// --------------------------------------------------------------------------
void Server::wait() {
  //do nothing
}

Server& Server::get(const std::string& path, route_cb&& cb) {
  Router::getInstance()->add_route(beast::http::verb::get, Route(path, std::move(cb)));
  return *this;
}

Server& Server::put(const std::string& path, route_cb&& cb) {
  Router::getInstance()->add_route(beast::http::verb::put, Route(path, std::move(cb)));
  return *this;
}

Server& Server::post(const std::string& path, route_cb&& cb) {
  Router::getInstance()->add_route(beast::http::verb::post, Route(path, std::move(cb)));
  return *this;
}

Server& Server::options(const std::string& path, route_cb&& cb) {
  Router::getInstance()->add_route(beast::http::verb::options, Route(path, std::move(cb)));
  return *this;
}

Server& Server::del(const std::string& path, route_cb&& cb) {
  Router::getInstance()->add_route(beast::http::verb::delete_, Route(path, std::move(cb)));
  return *this;
}

Server& Server::ws(const std::string& path, WsHandler&& handler) {
  Router::getInstance()->add_route(beast::http::verb::get, Route(path, std::move(handler)));
  return *this;
}

Server& Server::broadcast(std::function<void()> callbackFunction) {
  // Router::getInstance()->add_route(beast::http::verb::get, Route(path, std::move(handler)));
  callbackFunction();
  return *this;
}

