
#include <iostream>
#include "http_worker.hpp"
#include <boost/beast/http/empty_body.hpp>
#include "router.hpp"
#include "request.hpp"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include "../util/file_content.hpp"
#include "../util/string_util.hpp"
#include "../configuration/driver/ini.hpp"
#include "ws_worker.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>

using namespacedigital::cppserver::router::HttpWorker;
using configuration = namespacedigital::cppserver::configuration::Ini;
using namespacedigital::cppserver::router::Router;
using namespace namespacedigital::cppserver::util;

namespace fs = std::filesystem;

bool isStringNumber(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


beast::string_view mime_type(beast::string_view path) {
    using beast::iequals;
    auto const ext = [&path] {
        auto const pos = path.rfind(".");
        if (pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
        }();
        if (iequals(ext, ".htm"))  return "text/html";
        if (iequals(ext, ".mkv"))  return "video/mp4";
        if (iequals(ext, ".html")) return "text/html";
        if (iequals(ext, ".php"))  return "text/html";
        if (iequals(ext, ".css"))  return "text/css";
        if (iequals(ext, ".txt"))  return "text/plain";
        if (iequals(ext, ".js"))   return "application/javascript";
        if (iequals(ext, ".json")) return "application/json";
        if (iequals(ext, ".xml"))  return "application/xml";
        if (iequals(ext, ".swf"))  return "application/x-shockwave-flash";
        if (iequals(ext, ".flv"))  return "video/x-flv";
        if (iequals(ext, ".png"))  return "image/png";
        if (iequals(ext, ".jpe"))  return "image/jpeg";
        if (iequals(ext, ".jpeg")) return "image/jpeg";
        if (iequals(ext, ".jpg"))  return "image/jpeg";
        if (iequals(ext, ".gif"))  return "image/gif";
        if (iequals(ext, ".bmp"))  return "image/bmp";
        if (iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
        if (iequals(ext, ".tiff")) return "image/tiff";
        if (iequals(ext, ".tif"))  return "image/tiff";
        if (iequals(ext, ".svg"))  return "image/svg+xml";
        if (iequals(ext, ".svgz")) return "image/svg+xml";
        return "application/text";
}

Router* Router::instancePtr = NULL;

void HttpWorker::accept() {

    // Clean up any previous connection.
    beast::error_code ec;
    socket_.close(ec);
    buffer_.consume(buffer_.size());

    acceptor_.async_accept(
        socket_,
        [self = shared_from_this()](auto ec) {
            if (ec) {
                self->accept();
            } else {
                // Request must be fully processed within 60 seconds.
                self->request_deadline_.expires_after(
                    std::chrono::seconds(60));

                self->read_request();
            }
        });
}


void HttpWorker::read_request() {
    // On each read the parser needs to be destroyed and
    // recreated. We store it in a boost::optional to
    // achieve that.
    //
    // Arguments passed to the parser constructor are
    // forwarded to the message object. A single argument
    // is forwarded to the body constructor.
    //
    // We construct the dynamic body with a 1MB limit
    // to prevent vulnerability to buffer attacks.
    //
    parser_.emplace(
        std::piecewise_construct,
        std::make_tuple());

    http::async_read(
        socket_,
        buffer_,
        *parser_,
        asio::bind_executor(strand_, [this, self = this->shared_from_this()](auto ec, auto bytes_transferred) {

            if (ec) {
                self->accept();
            } else {
                try {

                    auto router = Router::getInstance();
                    auto& configuration = configuration::Ini::getInstance();
                    _request = self->parser_->release();
                    auto found_method = router->find(_request.method());
                    if (found_method == router->end()) {
                        // return helper::bad_request(_request, "Not supported HTTP-method");
                    }
                    // auto found_method = router->find(_request.method());
                    auto res = std::make_shared<Response>(beast::http::status::ok, _request.version());
                    bool isWebsocket = (beast::websocket::is_upgrade(_request));

                    res->set_http_worker(std::move(this));
                    for (auto&& route : found_method->second) {
                        if (route.match(_request)) {
                            try {
                                if (isWebsocket) {
                                    // Create a websocket session, and transferring ownership
                                    std::make_shared<WsWorker>(std::move(socket_), route)->run(_request);
                                    return;
                                } else {
                                    route.execute(_request, *res);
                                    //if response is populated inside route callback then send it
                                    if (res->body().size() > 0) {
                                        send_response(res.get());
                                    }
                                    return;
                                }
                            } catch (const std::exception& ex) {
                                std::cout << "Handler : " << ex.what() << std::endl;
                                throw std::runtime_error(ex.what());
                                self->send_bad_response(http::status::internal_server_error, "Internal server err\r\n");
                                return;
                            }
                        }
                    }

                    //check for static files
                    if (doc_root_.size()) {
                        auto target = _request.target();
                        std::string uri{ target };
                        std::string full_path = doc_root_;
                        full_path.append(
                            target.data(),
                            target.size());

                        //if file return it
                        FileType fileType = file_type(full_path, fs::status(full_path));
                        if (fileType == FileType::REGULAR_FILE) {

                            //check if range header is present
                            std::string start = "";
                            std::string end = "";
                            for (auto const& field : _request) {
                                //                                    std::cout << field.name() << " " << field.value() << std::endl;
                                if (field.name_string() == "Range") {
                                    start = field.value().data();
                                    replace(start, "bytes=", "");
                                    std::vector<std::string> results = split(start, "-");
                                    start = results.front();
                                    end = results.back();
                                    break;
                                }
                            }
                            //send file with range support
                            if (start != "" && end != "") {
                                res->result(http::status::partial_content);
                                res->keep_alive(true);
                                res->set(http::field::content_type, "video/mp4");
                                res->set(beast::http::field::accept_ranges, "bytes");
                                res->set(beast::http::field::connection, "Keep-Alive");
                                res->send(full_path, start, end);
                                return;
                            }
                            std::string extension = full_path.substr(full_path.size() - 4, full_path.size());
                            if (boost::beast::iequals(extension, ".srt")) {
                                auto subtitleEncodingConfig = configuration.getConfig("movie.subtitle_encoding");
                                if (subtitleEncodingConfig != "") {
                                    res->set(beast::http::field::content_type, "text/plain;charset=" + configuration.getConfig("movie.subtitle_encoding"));
                                }

                            }

                            res->sendFileBodyResponse(full_path);
                            return;
                        }

                        //scan the folder for folders and files
                        std::string htmlBody = get_html_directory_content(full_path.c_str(), uri);
                        res->body() = htmlBody;
                        send_response(res.get());
                        return;
                    }

                    send_bad_response(http::status::not_found, "ROute not found\r\n");
                } catch (const std::exception& ex) {
                    std::cout << "HttpWorker: " << ex.what() << std::endl;
                    socket_.close();
                    self->send_bad_response(http::status::not_found, "Error occured\n");
                    return;
                }
            }

            }));
}

void HttpWorker::write_empty_body_header() {
    http::response<http::empty_body> response{ http::status::ok, 11 };
    //    response.set(http::field::content_type, "video/mp4");
    http::response_serializer<http::empty_body> serializer{ response };
    http::write_header(socket_, serializer);
}

void HttpWorker::write_empty_body_header(beast::http::response<beast::http::empty_body>&& res) {
    http::response_serializer<http::empty_body> serializer{ res };
    http::write_header(socket_, serializer);
}

/**
 important for to be called not to block acceptor in async_write read
 */
void HttpWorker::handle_end_write() {
    beast::error_code ec;
    socket_.shutdown(tcp::socket::shutdown_send, ec);
    accept();
}
void HttpWorker::process_request(http::request<request_body_t> const& req) {

    //  std::string start = "";
    //  std::string end = "";
    //
    //  boost::beast::http::fields fields = req.base();
    //
    //  for (const boost::beast::http::fields::value_type& header : fields) {
    //    if (header.name_string() == "Range") {
    //      start = header.value().data();
    //      replace(start, "bytes=", "");
    //      std::vector<std::string> results = split(start, "-");
    //      start = results.front();
    //      end = results.back();
    //    }
    //  }
    switch (req.method()) {
    case http::verb::get:
        //    send_file(req.target(), start, end);
        break;

    case http::verb::post:
        //    send_file(req.target(), start, end);
        break;

    default:
        // We return responses indicating an error if
        // we do not recognize the request method.
        send_bad_response(
            http::status::bad_request,
            "Invalid request-method '" + std::string(req.method_string()) + "'\r\n");
        break;
    }
}

void HttpWorker::send_bad_response(http::status status, std::string const& error) {
    string_response_.emplace(std::piecewise_construct, std::make_tuple());

    string_response_->result(status);
    string_response_->keep_alive(false);
    string_response_->set(http::field::server, "Beast");
    string_response_->set(http::field::content_type, "text/plain");
    string_response_->body() = error;
    string_response_->prepare_payload();

    string_serializer_.emplace(*string_response_);

    http::async_write(
        socket_,
        *string_serializer_,
        asio::bind_executor(this->strand_,
            [self = this->shared_from_this()](auto ec, auto bytes_transferred) {
                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                self->string_serializer_.reset();
                self->string_response_.reset();
                self->accept();
            }
    ));
}


void HttpWorker::send_response(beast::http::response < beast::http::string_body>* res) {
    string_response_.emplace(std::piecewise_construct, std::make_tuple());

    string_response_->result(http::status::ok);
    string_response_->keep_alive(false);
    string_response_->set(http::field::server, "Beast");
    string_response_->body() = std::move(res->body());
    string_response_->set(http::field::access_control_allow_origin, "*");
    string_response_->prepare_payload();
    for (auto const& field : *res) {
        string_response_->set(field.name(), field.value());
    }
    string_serializer_.emplace(*string_response_);

    http::async_write(
        socket_,
        *string_serializer_,
        asio::bind_executor(this->strand_,
            [self = this->shared_from_this()](auto ec, auto bytes_transferred) {
                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                self->string_serializer_.reset();
                self->string_response_.reset();
                self->accept();
            }
    ));
}



void HttpWorker::send_buffer_body_response(const char buffer[], const size_t length) {
    net::const_buffer buf = { buffer, length };
    net::write(socket_, boost::asio::buffer(buf));
}

void HttpWorker::send_buffer_body_response_async(const char buffer[], const size_t length) {
    net::const_buffer buf = { buffer, length };
    net::async_write(socket_, boost::asio::buffer(buf), asio::bind_executor(this->strand_,
        [self = this->shared_from_this()](auto ec, auto bytes_transferred) {
            self->socket_.shutdown(tcp::socket::shutdown_send, ec);
            self->accept();
        }
    ));
}

void HttpWorker::send_file(beast::http::response <beast::http::file_body>* res) {
    beast::error_code ec;

    file_response_.emplace(std::piecewise_construct, std::make_tuple());
    file_response_->result(http::status::ok);
    file_response_->keep_alive(false);
    file_response_->body() = std::move(res->body());
    file_response_->set(http::field::access_control_allow_origin, "*");
    file_response_->prepare_payload();
    for (auto const& field : *res) {
        file_response_->set(field.name(), field.value());
    }
    file_serializer_.emplace(*file_response_);

    http::async_write(
        socket_,
        *file_serializer_,
        asio::bind_executor(this->strand_,
            [self = this->shared_from_this()](auto ec, auto bytes_transferred) {
                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                self->file_serializer_.reset();
                self->file_response_.reset();
                self->accept();
            }
    ));
}

void HttpWorker::check_deadline() {
    // The deadline may have moved, so check it has really passed.
    if (request_deadline_.expiry() <= std::chrono::steady_clock::now()) {
        // Close socket to cancel any outstanding operation.
        socket_.close();

        // Sleep indefinitely until we're given a new deadline.
        request_deadline_.expires_at(
            (std::chrono::steady_clock::time_point::max)());
    }

    request_deadline_.async_wait(
        [this](beast::error_code) {
            check_deadline();
        });
}


void HttpWorker::send_file(beast::http::response<beast::http::file_body>* res, const fs::path& path, std::string& start, std::string& end) {
    http::file_body::value_type file;
    beast::error_code ec;

    //open file
    file.open(path.c_str(), beast::file_mode::read, ec);



    if (ec) {
        send_bad_response(http::status::not_found, "Erorr happened while open the file\r\n");
        return;
    }

    if (!file.is_open()) {
        send_bad_response(http::status::not_found, "File not found\r\n");
        return;
    }

    size_t file_size = file.size();

    //calculate range
    size_t startInt = isStringNumber(start) ? stol(start) : 0;
    std::cout << "start" << startInt << std::endl;
    size_t endInt = isStringNumber(end) ? stol(end) : file_size - 1;
    std::cout << "end" << endInt << std::endl;


    //get the portion of file
    file.file().seek(startInt, ec);


    std::string fullRange = "";
    fullRange.append("bytes ");
    fullRange.append(std::to_string(startInt));
    fullRange.append("-");
    fullRange.append(std::to_string(endInt));
    fullRange.append("/");
    fullRange.append(std::to_string(file_size));



    file_response_.emplace(std::piecewise_construct, std::make_tuple());

    std::string contentLength = std::to_string(endInt - startInt + 1);
    std::cout << "length" << contentLength << std::endl;
    std::cout << "range" << fullRange << std::endl;

    // res.result(http::status::partial_content);
    // file_response_->result(http::status::ok);
    file_response_->result(res->result());
    file_response_->keep_alive(res->keep_alive());
    file_response_->set(http::field::server, "Beast");
    file_response_->set(http::field::content_type, mime_type(std::string(path)));
    file_response_->set(beast::http::field::content_length, contentLength);
    file_response_->set(beast::http::field::content_range, fullRange);
    file_response_->body() = std::move(file);
    // file_response_->prepare_payload();

    for (auto const& field : *res) {
        std::cout << field.name() << " " << field.value() << std::endl;
        file_response_->set(field.name(), field.value());
    }
    file_serializer_.emplace(*file_response_);

    http::async_write(
        socket_,
        *file_serializer_,
        asio::bind_executor(this->strand_,
            [self = this->shared_from_this()](auto ec, auto bytes_transferred) {
                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                self->file_serializer_.reset();
                self->file_response_.reset();
                self->accept();
            }
    ));
}
