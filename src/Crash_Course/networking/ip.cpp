#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

namespace asio = boost::asio;
namespace ip = asio::ip;
using std::cout, std::endl, std::string, std::cerr, std::vector;

/* IP Suite */

// Hostname Resolution
// int main(int argc, char* argv[]) {
//     asio::io_context context;
//     ip::tcp::resolver resolver{context};
//     boost::system::error_code ec;
//
//     for (auto&& result : resolver.resolve("www.nostarch.com", "http", ec)) {
//         cout << result.service_name() << " " << result.host_name() << " " << result.endpoint() << endl;
//     }
//     if (ec) cerr << "Error code: " << ec << endl;
//
//     resolver.async_resolve("www.nostarch.com", "http", [](boost::system::error_code ec, const auto& results) {
//         if (ec) cerr << "Error code: " << ec << endl;
//
//         for (auto&& result : results) {
//             cout << result.service_name() << " " << result.host_name() << " " << result.endpoint() << endl;
//         }
//     });
//
//     context.run();
//     return 0;
// }

// Connecting
int main() {
    asio::io_context context;
    ip::tcp::socket sock{context};

    auto localhost = asio::ip::make_address("127.0.0.1");
    vector<ip::basic_endpoint<ip::tcp>> ends{{localhost, 6600}, {localhost, 5433}};
    try {
        const auto connected_endpoint = asio::connect(sock, ends);
        cout << connected_endpoint << endl;
    } catch (boost::system::system_error& e) {
        cerr << "Error: " << e.what() << endl;
    }

    // asio::async_connect(sock, ends, [](boost::system::error_code ec, const auto& endpoint) { cout << endpoint; });
    // context.run();

    return 0;
}

/* IP Suite */
