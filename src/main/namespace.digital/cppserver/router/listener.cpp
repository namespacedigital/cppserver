#include "listener.hpp"
#include "http_worker.hpp"
#include "mime_type.hpp"
#include <iostream>
#include <cstdlib>

using namespacedigital::cppserver::router::Listener;
using namespacedigital::cppserver::router::HttpWorker;

Listener::Listener(
  asio::io_context& ioc,
  tcp::endpoint endpoint,
  std::shared_ptr<std::string const> const& doc_root)
  : ioc_(ioc)
  , acceptor_(net::make_strand(ioc))
  , doc_root_(doc_root) {
  beast::error_code ec;

  // Open the acceptor
  acceptor_.open(endpoint.protocol(), ec);
  if (ec) {
    fail(ec, "open");
    return;
  }

  // Allow address reuse
  acceptor_.set_option(net::socket_base::reuse_address(true), ec);
  if (ec) {
    fail(ec, "set_option");
    return;
  }

  // Bind to the server address
  acceptor_.bind(endpoint, ec);
  if (ec) {
    fail(ec, "bind");
    return;
  }

  // Start listening for connections
  acceptor_.listen(
    net::socket_base::max_listen_connections, ec);
  if (ec) {
    fail(ec, "listen");
    return;
  }
}

// Start accepting incoming connections
void Listener::run() {
  // We need to be executing within a strand to perform async operations
  // on the I/O objects in this session. Although not strictly necessary
  // for single-threaded contexts, this example code is written to be
  // thread-safe by default.
  net::dispatch(
    acceptor_.get_executor(),
    beast::bind_front_handler(
      &Listener::do_accept,
      this->shared_from_this()));
}

void Listener::do_accept() {
  // The new connection gets its own strand
  acceptor_.async_accept(
    net::make_strand(ioc_),
    beast::bind_front_handler(
      &Listener::on_accept,
      shared_from_this()));
}

/**
 * socket  The socket for the currently connected client.
*/
void Listener::on_accept(beast::error_code ec, tcp::socket socket) {
  if (ec) {
    fail(ec, "accept");
  } else {
    // // Create the http session and run it
    // std::make_shared<http_session>(
    //     std::move(socket),
    //     std::move(doc_root_))->run();

    std::make_shared<HttpWorker>(
      std::move(socket),
      std::move(doc_root_), ioc_)->run();
  }

  // Accept another connection
  do_accept();
}

// Report a failure
void Listener::fail(beast::error_code ec, char const* what) {
  std::cerr << what << ": " << ec.message() << "\n";
}

