#include <boost/algorithm/string/case_conv.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <sstream>
#include <string>

namespace asio = boost::asio;
namespace sys = boost::system;
namespace ip = asio::ip;
using std::cout, std::endl, std::string, std::cerr, std::ostream, std::istream, std::stringstream, ip::udp,
    boost::algorithm::to_upper;

void handle(udp::socket& sock) {
    sys::error_code ec;
    string message;
    do {
        udp::endpoint remote;
        sock.receive_from(asio::buffer(message), remote);
        // if (ec.value() == 2) return;
        cout << "[" << sock.remote_endpoint() << "]: handle(): " << message << endl;
        to_upper(message);
        sock.send_to(asio::buffer(message), remote, 0, ec);
        // if (message == "\n") return;
        message.clear();
    } while (true);
}

int main() {
    try {
        asio::io_context context;
        const int port{9000};
        const auto host = udp::v4();

        udp::socket sock(context, udp::endpoint(udp::v4(), 9000));
        while (true) {
            handle(sock);
        }

    } catch (const sys::system_error& se) {
        cerr << "Error: " << se.what() << endl;
    }
}
