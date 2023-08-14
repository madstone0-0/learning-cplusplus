#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/test/execution_monitor.hpp>
#include <chrono>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace ip = asio::ip;
using std::cout, std::endl, std::string, std::cerr, std::ostream, std::istream, std::stringstream, ip::tcp;

struct Request : std::enable_shared_from_this<Request> {
    explicit Request(asio::io_context& context, std::string_view host)
        : request{http::verb::get, "/", 11},
          resolver{asio::make_strand(context)},
          stream{asio::make_strand(context)},
          host{std::move(host)} {
        request.set(http::field::host, host);
        request.set(http::field::accept, "text/html");
        request.set(http::field::accept_encoding, "identity");
        request.set(http::field::user_agent, "C++ Crash Course Client");
        request.set(http::field::connection, "close");
        cout << "Request:\n" << request << endl;

        resolver.async_resolve(this->host, "http",
                               [this](beast::error_code ec, const tcp::resolver::results_type& results) {
                                   resolutionHandler(ec, results);
                               });
    }

    void resolutionHandler(beast::error_code ec, const tcp::resolver::results_type& results) {
        if (ec) {
            cerr << "Error resolving " << host << ": " << ec << endl;
            return;
        }
        stream.async_connect(results, beast::bind_front_handler(&Request::connectionHandler, shared_from_this()));
    }

    void connectionHandler(beast::error_code ec, const tcp::endpoint& endpoint) {
        if (ec) {
            cerr << "Error connecting to " << host << ": " << ec << endl;
            return;
        }

        http::async_write(stream, request, beast::bind_front_handler(&Request::writeHandler, shared_from_this()));
    }

    void writeHandler(beast::error_code ec, size_t transferred) {
        if (ec) {
            cerr << "Error writing to " << host << ": " << ec << endl;
            return;
        }

        http::async_read(stream, buffer, response,
                         beast::bind_front_handler(&Request::readHandler, shared_from_this()));
    }

    void readHandler(beast::error_code ec, size_t transferred) {
        if (ec && ec.value() != 2) cerr << "Error reading from " << host << ": " << ec.message() << endl;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);
    }

    auto getResponse() { return response; }

   private:
    http::request<http::string_body> request;
    http::response<http::string_body> response;
    beast::flat_buffer buffer;

    tcp::resolver resolver;
    beast::tcp_stream stream;
    std::string_view host;
};

int main() {
    asio::io_context context;
    auto request = std::make_shared<Request>(context, "www.google.com");
    context.run();
    cout << "Response:\n" << request->getResponse() << endl;
}
