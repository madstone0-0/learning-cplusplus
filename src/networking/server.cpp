#include <boost/algorithm/string/case_conv.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
// #include <vector>
#include <sstream>

namespace asio = boost::asio;
namespace sys = boost::system;
namespace ip = asio::ip;
using std::cout, std::endl, std::string, std::cerr, std::ostream, std::istream, std::stringstream, ip::tcp,
    boost::algorithm::to_upper;

void handle(tcp::socket& sock) {
    sys::error_code ec;
    string message;
    do {
        asio::read_until(sock, asio::dynamic_buffer(message), "\n", ec);
        if (ec.value() == 2) return;
        cout << "[" << sock.remote_endpoint() << "]: handle(): " << message << endl;
        to_upper(message);
        asio::write(sock, asio::buffer("\n" + message), ec);
        if (message == "\n") return;
        message.clear();
    } while (ec.value() == 2 || !ec);
}

int main() {
    try {
        asio::io_context context;
        const int port{1895};

        tcp::acceptor acceptor{context, tcp::endpoint(ip::tcp::v4(), port)};
        auto host = ip::tcp::v4();
        cout << "Listening on localhost:" << port << endl;

        while (true) {
            tcp::socket sock{context};
            acceptor.accept(sock);
            handle(sock);
        }

    } catch (sys::system_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}
