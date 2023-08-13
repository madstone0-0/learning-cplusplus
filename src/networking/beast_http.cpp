#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/test/execution_monitor.hpp>
#include <chrono>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace ip = asio::ip;
using std::cout, std::endl, std::string, std::cerr, std::ostream, std::istream, std::stringstream, ip::tcp;

http::response<http::string_body> request(string host, asio::io_context& context) {
    http::request<http::string_body> req{http::verb::get, "/", 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::accept, "text/html");
    req.set(http::field::accept_language, "en-us");
    req.set(http::field::accept_encoding, "identity");
    req.set(http::field::connection, "close");

    cout << "Request:\n" << req << endl;

    tcp::resolver resolver{context};
    const auto endpoints = resolver.resolve(host, "http");
    tcp::socket sock{context};

    const auto connectedEndpoints = asio::connect(sock, endpoints);
    http::write(sock, req);

    http::response<http::string_body> res;
    beast::flat_buffer buffer;
    beast::error_code ec;
    http::read(sock, buffer, res, ec);
    sock.shutdown(tcp::socket::shutdown_both, ec);
    if (ec && ec.value() != 2) throw beast::system_error{ec};
    return res;
}

int main() {
    asio::io_context context;
    try {
        const auto res = request("www.google.com", context);
        cout << "Response:\n" << res << endl;
    } catch (const beast::system_error& se) {
        cerr << "Error: " << se.what() << endl;
    }
}
