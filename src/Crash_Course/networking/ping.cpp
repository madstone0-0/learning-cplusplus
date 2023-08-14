#include <algorithm>
#include <array>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace asio = boost::asio;
namespace sys = boost::system;
namespace ip = asio::ip;
using std::cout, std::endl, std::string, std::cerr, std::ostream, std::istream, std::stringstream, ip::icmp,
    std::vector, std::string_view;

void ping(asio::io_context& context, string_view host) {
    icmp::resolver resolver{context};
    auto desination = resolver.resolve(host, "");

    cout << desination->host_name() << endl;
}

void startSend() { string body{"Ping"}; }

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage ping ADDRESS_START" << endl;
        return -1;
    }

    asio::io_context context;

    string addr = argv[1];
    vector<string> components;
    boost::algorithm::split(components, addr, boost::is_punct());

    const auto last = std::stoul(components.back());
    components.erase(components.end() - 1);

    vector<int> addressEnds;

    auto start{last};
    auto range{256 - last % 256};

    vector<string> addresses(range);
    std::generate_n(std::back_inserter(addressEnds), range, [&start]() { return start++; });

    auto i{0};
    auto make_address = [&i, &components, &addressEnds]() {
        stringstream newAddrSS;
        auto res = boost::join(components, ".");
        newAddrSS << res;
        newAddrSS << "." << addressEnds[i];
        i++;
        return newAddrSS.str();
    };

    std::generate(addresses.begin(), addresses.end(), make_address);

    try {
        for (auto& host : addresses) {
            ping(context, host);
        }

    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
