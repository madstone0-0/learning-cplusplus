#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
// #include <vector>
#include <sstream>

namespace asio = boost::asio;
using std::cout, std::endl, std::string, std::cerr, std::vector, std::ostream, std::istream, std::stringstream;

string request(string host, asio::io_context& context) {
    stringstream request_stream;
    request_stream << "GET / HTTP/1.1\r\n"
                      "Host: "
                   << host
                   << "\r\n"
                      "Accept: text/html\r\n"
                      "Accept-Languge: en-us\r\n"
                      "Accept-Encoding: identity\r\n"
                      "Connection: close\r\n\r\n";
    const auto request = request_stream.str();

    asio::ip::tcp::resolver resolver{context};
    const auto endpoints = resolver.resolve(host, "http");
    asio::ip::tcp::socket sock{context};

    const auto connectedEndpoints = asio::connect(sock, endpoints);
    asio::write(sock, asio::buffer(request));
    string response;
    boost::system::error_code ec;
    asio::read(sock, asio::dynamic_buffer(response), ec);
    if (ec && ec.value() != 2) throw boost::system::system_error{ec};
    return response;
}

int main() {
    asio::io_context context;
    try {
        // const auto res = request("www.arcyber.army.mil", context);
        const auto res = request("www.google.com", context);
        cout << res << endl;
    } catch (boost::system::system_error& se) {
        cerr << "Error: " << se.what() << endl;
    }
}
