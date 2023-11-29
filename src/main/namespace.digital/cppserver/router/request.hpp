#ifndef NAMESPACE_DIGITAL_REQUEST_HPP
#define NAMESPACE_DIGITAL_REQUEST_HPP

#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "attributes.hpp"

namespace namespacedigital {
  namespace cppserver {
    namespace router {
      namespace beast = boost::beast;
      namespace http = beast::http;

      using endpoint = boost::asio::ip::tcp::endpoint;

      class Request : public beast::http::request<beast::http::string_body> {
      public:

        using beast::http::request<beast::http::string_body>::request;
        using beast::http::request<beast::http::string_body>::operator=;

        Attributes& get_attributes() { return _attributes; }
        const Attributes& get_attributes() const { return _attributes; }
        const Attribute& a(const std::string& key) const { return _attributes[key]; }

        const endpoint& remote() const { return _remote_ep; }
        void remote(endpoint ep) { _remote_ep = std::move(ep); }

      private:
        Attributes  _attributes;
        endpoint    _remote_ep;
      };
    }
  }
}

#endif /* example_hpp */

