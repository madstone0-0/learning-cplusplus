#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
// #include <vector>
#include <sstream>

namespace asio = boost::asio;
using std::cout, std::endl, std::string, std::cerr, std::ostream, std::istream, std::stringstream;

using ResolveResult = boost::asio::ip::tcp::resolver::results_type;
using Endpoint = boost::asio::ip::tcp::endpoint;

struct Request {
    explicit Request(asio::io_context& context, string host) : resolver{context}, sock{context}, host{std::move(host)} {
        stringstream request_stream;
        request_stream << "GET / HTTP/1.1\r\n"
                          "Host: "
                       << this->host
                       << "\r\n"
                          "Accept: text/html\r\n"
                          "Accept-Languge: en-us\r\n"
                          "Accept-Encoding: identity\r\n"
                          "User-Agent: C++ Crash Course Client\r\n"
                          "Connection: close\r\n\r\n";
        request = request_stream.str();
        this->resolver.async_resolve(
            this->host, "http",
            [this](boost::system::error_code ec, const ResolveResult& results) { resolutionHandler(ec, results); });
    }

    void resolutionHandler(boost::system::error_code ec, const ResolveResult& results) {
        if (ec) {
            cerr << "Error resolving " << host << ": " << ec << endl;
            return;
        }
        asio::async_connect(sock, results, [this](boost::system::error_code ec, const Endpoint& endpoint) {
            connectionHandler(ec, endpoint);
        });
    }

    void connectionHandler(boost::system::error_code ec, const Endpoint& endpoint) {
        if (ec) {
            cerr << "Error conncting to " << host << ": " << ec << endl;
            return;
        }
        asio::async_write(sock, asio::buffer(request),
                          [this](boost::system::error_code ec, size_t transferred) { writeHandler(ec, transferred); });
    }

    void writeHandler(boost::system::error_code ec, size_t transferred) {
        if (ec) {
            cerr << "Error writing to " << host << ": " << ec << endl;
            return;
        } else if (request.size() != transferred) {
            request.erase(0, transferred);
            asio::async_write(sock, asio::buffer(request), [this](boost::system::error_code ec, size_t transferred) {
                writeHandler(ec, transferred);
            });
        } else {
            asio::async_read(
                sock, asio::dynamic_buffer(response),
                [this](boost::system::error_code ec, size_t transferred) { readHandler(ec, transferred); });
        }
    }

    void readHandler(boost::system::error_code ec, size_t transferred) {
        if (ec && ec.value() != 2) cerr << "Error reading from " << host << ": " << ec.message() << endl;
    }

    const string getResponse() const noexcept { return response; }

   private:
    string response;
    string request;

    asio::ip::tcp::resolver resolver;
    asio::ip::tcp::socket sock;
    const string host;
};

int main() {
    asio::io_context context;
    Request request{context, "www.google.com"};
    context.run();
    cout << request.getResponse() << endl;

    return 0;
}
