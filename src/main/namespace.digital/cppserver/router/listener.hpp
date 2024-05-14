#ifndef listener_hpp
#define listener_hpp

#include <boost/beast/http.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace namespacedigital {
  namespace cppserver {
    namespace router {

      namespace beast = boost::beast;       // from <boost/beast.hpp>
      namespace net = boost::asio;            // from <boost/asio.hpp>
      namespace asio = boost::asio;         // from <boost/beast.hpp>
      using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

      class Listener : public std::enable_shared_from_this<Listener> {
        asio::io_context& ioc_;
        // The acceptor used to listen for incoming connections.
        tcp::acceptor acceptor_;
        std::shared_ptr<std::string const> doc_root_;

      public:
        Listener(
          asio::io_context& ioc,
          tcp::endpoint endpoint,
          std::shared_ptr<std::string const> const& doc_root);

        // Start accepting incoming connections
        void run();
      private:
        void do_accept();

        void on_accept(beast::error_code ec, tcp::socket socket);

        void fail(beast::error_code ec, char const* what);
      };

    }
  }
}


#endif