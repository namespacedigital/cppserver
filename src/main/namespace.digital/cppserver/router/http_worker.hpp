#ifndef http_worker_hpp
#define http_worker_hpp

#include <boost/beast/http.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <filesystem>
#include "request.hpp"
// #include "handler.hpp"
// #include "router.hpp"

namespace namespacedigital {
    namespace cppserver {
        namespace router {

            namespace asio = boost::asio;         // from <boost/beast.hpp>
            namespace beast = boost::beast;         // from <boost/beast.hpp>
            namespace http = beast::http;           // from <boost/beast/http.hpp>
            namespace net = boost::asio;            // from <boost/asio.hpp>
            namespace fs = std::filesystem;
            using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


            class HttpWorker : public std::enable_shared_from_this<HttpWorker> {
            public:
                //using request_body_t = http::basic_dynamic_body<beast::flat_static_buffer<1024 * 1024>>;
                using request_body_t = http::string_body;
                // The path to the root of the document directory.
                std::shared_ptr<std::string const> doc_root_;
                // The parser for reading the requests
                boost::optional<http::request_parser<request_body_t>> parser_;

                HttpWorker(asio::ip::tcp::socket&& socket, std::shared_ptr<std::string const> const& doc_root, asio::io_context& ioc) :
                    socket_(std::move(socket)),
                    doc_root_(doc_root),
                    strand_(asio::make_strand(ioc)) {
                }

                HttpWorker(HttpWorker const&) = delete;
                HttpWorker& operator=(HttpWorker const&) = delete;

                // Start the session
                void run();


                void write_empty_body_header();
                void write_empty_body_header(beast::http::response<beast::http::empty_body>&& res);
                void handle_end_write();
                void send_response(beast::http::response<beast::http::string_body>* res);
                void send_file(beast::http::response <beast::http::file_body>* res);
                void send_file(beast::http::response<beast::http::file_body>* res, const fs::path& path, std::string& start, std::string& end);
                void send_buffer_body_response(const char buffer[], const size_t length);
                void send_buffer_body_response_async(const char buffer[], const size_t length);
                void send_bad_response(http::status status, std::string const& error);

            private:
                Request     _request;
                asio::strand<asio::io_context::executor_type> strand_;

                // The socket for the currently connected client.
                asio::ip::tcp::socket socket_;


                // The buffer for performing reads
                beast::flat_static_buffer<8192> buffer_;

                // The file-based response message.
                boost::optional<http::response<http::file_body>> file_response_;

                // The file-based response message.
                boost::optional<http::response<http::buffer_body>> buffer_response_;


                // The string-based response message.
                boost::optional<http::response<http::string_body>> string_response_;

                // The string-based response serializer.
                boost::optional<http::response_serializer<http::string_body>> string_serializer_;

                // The file-based response serializer.
                boost::optional<http::response_serializer<http::file_body>> file_serializer_;

                // The dynamic-buffer-based response serializer.
                boost::optional<http::response_serializer<http::buffer_body>> buffer_serializer_;

                void read_request();

                void process_request(http::request<request_body_t> const& req);
            };
        }
    }

}
#endif /* http_worker */
