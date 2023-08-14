#include <boost/algorithm/string/case_conv.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
// #include <vector>
#include <sstream>

namespace asio = boost::asio;
namespace sys = boost::system;
namespace ip = asio::ip;
using std::cout, std::endl, std::string, std::cerr, std::ostream, std::istream, std::stringstream, ip::tcp,
    boost::algorithm::to_upper;

struct Session : std::enable_shared_from_this<Session> {
    explicit Session(tcp::socket sock) : sock{std::move(sock)} {}

    void read() {
        asio::async_read_until(
            sock, asio::dynamic_buffer(message), "\n", [self = shared_from_this()](sys::error_code ec, size_t length) {
                if (ec || self->message == "\n") return;
                cout << "[" << self->sock.remote_endpoint() << "]: handle(): " << self->message << endl;
                to_upper(self->message);
                self->write();
            });
    }

    void write() {
        asio::async_write(sock, asio::buffer("\n" + message),
                          [self = shared_from_this()](sys::error_code ec, size_t length) {
                              if (ec) return;
                              self->message.clear();
                              self->read();
                          });
    }

   private:
    tcp::socket sock;
    string message;
};

void serve(tcp::acceptor& acceptor) {
    acceptor.async_accept([&acceptor](sys::error_code ec, tcp::socket sock) {
        serve(acceptor);
        if (ec) return;
        auto session = std::make_shared<Session>(std::move(sock));
        session->read();
    });
}

int main() {
    try {
        asio::io_context context;
        const int port{1895};
        const auto host = tcp::v4();

        tcp::acceptor acceptor{context, tcp::endpoint{tcp::v4(), port}};
        serve(acceptor);
        cout << "Listening on localhost:" << port << endl;

        context.run();
    } catch (sys::system_error& se) {
        cerr << "Error: " << se.what() << endl;
    }

    return 0;
}
