#ifndef NAMESPACE_DIGITAL_RESPONSE_HPP
#define NAMESPACE_DIGITAL_RESPONSE_HPP
#include <boost/beast/http.hpp>
#include "header.hpp"
#include "http_worker.hpp"
#include <iostream>


namespace namespacedigital {
    namespace cppserver {
        namespace router {

            namespace fs = std::filesystem;
            namespace beast = boost::beast;
            namespace http = beast::http;

            class Response : public beast::http::response<beast::http::string_body> {
            public:
                using beast::http::response<beast::http::string_body>::response;
                using beast::http::response<beast::http::string_body>::set;
                using beast::http::response<beast::http::string_body>::operator=;

                void set(const namespacedigital::cppserver::router::header::content_type& ct) {
                    set(beast::http::field::content_type, ct.value);
                }

                void set_header(beast::http::field name, const std::string& value) {
                    set(name, value);
                }

                bool is_postponed() const { return _is_postponed; }
                void postpone() { _is_postponed = true; }

                void done() { _cb(); _cb = [] {}; }

                void set_http_worker(HttpWorker* httpWorker) {
                    _http_worker = httpWorker;
                }
                void writeHeader() {
                    this->_http_worker->write_empty_body_header();
                }

                void writeHeader(beast::http::response<beast::http::empty_body>&& res) {
                    this->_http_worker->write_empty_body_header(std::move(res));
                }

                void writeEnd() {
                    this->_http_worker->handle_end_write();
                }

                void async_write(const char buffer[], const size_t length) {
                    this->_http_worker->send_buffer_body_response_async(buffer, length);
                }

                void write(const char buffer[], const size_t length) {
                    this->_http_worker->send_buffer_body_response(buffer, length);
                }

                void sendFileBodyResponse(const fs::path& path) {
                    auto res = std::make_shared<beast::http::response<beast::http::file_body>>();

                    http::file_body::value_type file;
                    beast::error_code ec;

                    file.open(path.c_str(), beast::file_mode::read, ec);

                    if (ec) {
                        this->_http_worker->send_bad_response(http::status::not_found, "Erorr happened while open the file\r\n");
                        return;
                    }

                    if (!file.is_open()) {
                        this->_http_worker->send_bad_response(http::status::not_found, "File not found\r\n");
                        return;
                    }

                    res->body() = std::move(file);

                    for (auto const& field : *this) {
                        res->set(field.name(), field.value());
                    }
                    this->_http_worker->send_file(res.get());

                }

                void send(const fs::path& path, std::string& start, std::string& end) {
                    auto res = std::make_shared<beast::http::response<beast::http::file_body>>();

                    res->result(this->result());
                    res->keep_alive(this->keep_alive());

                    for (auto const& field : *this) {
                        res->set(field.name(), field.value());
                    }
                    this->_http_worker->send_file(res.get(), path, start, end);

                }

                void send() {
                    this->_http_worker->send_response(this);
                }

                void send(const fs::path& path) {
                    auto res = std::make_shared<beast::http::response<beast::http::file_body>>();

                    res->result(this->result());
                    res->keep_alive(this->keep_alive());

                    for (auto const& field : *this) {
                        res->set(field.name(), field.value());
                    }

                }

                void on_done(std::function<void()>&& cb) {
                    _cb = std::move(cb);
                }

                http::status status() const { return result(); }
                bool is_status_ok() const { return status() == http::status::ok; }

            private:
                bool    _is_postponed = false;
                HttpWorker* _http_worker;
                std::function<void()> _cb = []() {};
            };

        }
    }
}
#endif 
