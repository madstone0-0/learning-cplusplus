#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

namespace asio = boost::asio;
namespace ip = asio::ip;
using std::cout, std::endl, std::string, std::cerr, std::vector, ip::tcp, std::istream, std::ostream;

auto makeTimer(asio::io_context& context) {
    return asio::steady_timer{context, std::chrono::steady_clock::now() + std::chrono::milliseconds(5)};
}

int main() {
    asio::io_context context;
    tcp::socket sock{context};
    auto localhost = asio::ip::make_address("127.0.0.1");
    asio::streambuf sb;
    try {
        vector<ip::basic_endpoint<ip::tcp>> endpoints{{localhost, 8888}, {localhost, 5433}};
        auto connectedEndpoint = asio::connect(sock, endpoints);
        cout << connectedEndpoint << endl;
        ostream os{&sb};

        for (auto i{0}; i <= 10; i++) {
            os << "SHADOW WIZARD MONEY GANG WE LOVE CASTING SPELLS\n";
            asio::write(sock, sb);
            makeTimer(context).wait();
        }

        // readTimer.wait();
        asio::read(sock, sb, asio::transfer_at_least(1024));
        cout << string{asio::buffers_begin(sb.data()), asio::buffers_end(sb.data())} << endl;
        // cout << connectedEndpoint << endl;

    } catch (boost::system::system_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
}
