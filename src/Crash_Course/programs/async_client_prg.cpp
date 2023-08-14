#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/program_options.hpp>
#include <chrono>
#include <exception>
#include <future>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace asio = boost::asio;
namespace po = boost::program_options;
using std::cout, std::endl, std::string, std::cerr, std::ostream, std::istream, std::stringstream;

using ResolveResult = boost::asio::ip::tcp::resolver::results_type;
using Endpoint = boost::asio::ip::tcp::endpoint;

struct Request {
    explicit Request(asio::io_context& context, string host, std::vector<string>& targets)
        : responses(targets.size()),
          requests(targets.size()),
          resolver{context},
          sock{context},
          host{std::move(host)},
          targets{std::move(targets)} {
        std::transform(this->targets.begin(), this->targets.end(), requests.begin(), [this](auto target) {
            stringstream request_stream;
            request_stream << "GET " << target
                           << " HTTP/1.1\r\n"
                              "Host: "
                           << this->host
                           << "\r\n"
                              "Accept: text/html\r\n"
                              "Accept-Languge: en-us\r\n"
                              "Accept-Encoding: identity\r\n"
                              "User-Agent: C++ Crash Course Client\r\n"
                              "Connection: close\r\n\r\n";
            auto request = request_stream.str();
            cout << "Request:\n" << request << "\n";
            return request;
        });

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
        for (auto& request : requests) {
            asio::async_write(sock, asio::buffer(request), [this](boost::system::error_code ec, size_t transferred) {
                writeHandler(ec, transferred);
            });
        }
        // asio::async_write(sock, asio::buffer(requests),
        //                   [this](boost::system::error_code ec, size_t transferred) { writeHandler(ec, transferred);
        //                   });
    }

    void writeHandler(boost::system::error_code ec, size_t transferred) {
        // if (ec) {
        //     cerr << "Error writing to " << host << ": " << ec << endl;
        //     return;
        // } else if (requests.size() != transferred / targets.size()) {
        //     requests.erase(requests.begin(), requests.end());
        //     for (auto& request : requests) {
        //         asio::async_write(
        //             sock, asio::buffer(request),
        //             [this](boost::system::error_code ec, size_t transferred) { writeHandler(ec, transferred); });
        //     }
        // } else {
        //     for (auto& response : responses) {
        //         asio::async_read(
        //             sock, asio::dynamic_buffer(response),
        //             [this](boost::system::error_code ec, size_t transferred) { readHandler(ec, transferred); });
        //     }
        // }
        if (ec) {
            cerr << "Error writing to " << host << ": " << ec << endl;
            return;
        }
        for (auto& response : responses) {
            asio::async_read(sock, asio::dynamic_buffer(res),
                             [this, &response](boost::system::error_code ec, size_t transferred) {
                                 response = res;
                                 readHandler(ec, transferred);
                             });
        }
    }

    void readHandler(boost::system::error_code ec, size_t transferred) {
        if (ec && ec.value() != 2) cerr << "Error reading from " << host << ": " << ec.message() << endl;
    }

    auto getResponses() const noexcept { return responses; }

   private:
    string res;
    std::vector<string> responses;
    std::vector<string> requests;

    asio::ip::tcp::resolver resolver;
    asio::ip::tcp::socket sock;
    const string host;
    const std::vector<string> targets;
};

namespace std {
    template <typename T>
    ostream& operator<<(ostream& os, const std::vector<T>& vec) {
        for (const auto& item : vec) os << "( " << item << " ";
        os << ")";
        return os;
    }
}  // namespace std

int main(int argc, char** argv) {
    bool isHelp{};

    po::options_description description;
    description.add_options()("help,h", po::bool_switch(&isHelp), "display help message")(
        "host,o", po::value<string>(), "Host to connect to")("targets", po::value<std::vector<string>>(),
                                                             "Targets on the host");
    po::positional_options_description positional;
    positional.add("targets", -1);

    po::command_line_parser parser{argc, argv};
    parser.options(description);
    parser.positional(positional);

    po::variables_map vm;
    try {
        auto parserRes = parser.run();
        po::store(parserRes, vm);
        po::notify(vm);
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    if (isHelp) {
        cout << description;
        return 0;
    }

    if (vm["targets"].empty()) {
        cerr << "You must provide a target to fetch" << endl;
        return -1;
    }

    if (vm["host"].empty()) {
        cerr << "You must provide a host to connect to" << endl;
        return -1;
    }

    try {
        // const int nThreads{3};
        asio::io_context context;
        const auto host = vm["host"].as<string>();
        auto targets = vm["targets"].as<std::vector<string>>();

        Request request{context, host, targets};

        context.run();

        cout << "Responses:\n";
        for (const auto& response : request.getResponses()) {
            cout << response << "\n";
        }

    } catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
