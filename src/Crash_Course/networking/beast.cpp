#include <signal.h>

#include <algorithm>
#include <atomic>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast.hpp>
#include <boost/program_options.hpp>
#include <chrono>
#include <exception>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>

namespace asio = boost::asio;
namespace op = boost::program_options;
namespace fs = std::filesystem;
namespace beast = boost::beast;
namespace ch = std::chrono;
namespace http = beast::http;

using std::cout, std::endl, std::string, std::cerr, std::vector, std::ostream, std::istream, std::stringstream,
    std::fstream, asio::ip::tcp, std::string_view, std::future;

string_view resolveMIMEType(const fs::path path) {
    const string ext = path.extension();
    if (ext == ".html" || ext == ".htm" || ext == ".php") return "text/html";
    if (ext == ".css") return "text/css";
    if (ext == ".txt") return "text/plain";
    if (ext == ".js") return "application/javascript";
    if (ext == ".json") return "application/json";
    if (ext == ".xml") return "application/xml";
    if (ext == ".swf") return "application/x-shockwave-flash";
    if (ext == ".flv") return "video/x-flv";
    if (ext == ".png") return "image/png";
    if (ext == ".jpe" || ext == ".jpeg" || ext == ".jpg") return "image/jpeg";
    if (ext == ".gif") return "image/gif";
    if (ext == ".bmp") return "image/bmp";
    if (ext == ".ico") return "image/vnd.microsoft.icon";
    if (ext == ".tiff" || ext == ".tif") return "image/tiff";
    if (ext == ".svg" || ext == ".svgz") return "image/svg+xml";
    return "application/text";
}

string formatMsg(string_view subject, string_view message) noexcept {
    stringstream ss;
    auto now = ch::system_clock::now();
    auto nowTT = ch::system_clock::to_time_t(now);
    auto timeInfo = std::put_time(std::localtime(&nowTT), "%c");
    ss << "(" << timeInfo << ") "
       << "[" << subject << "]: " << message << "\n";
    return ss.str();
}

string pathCat(fs::path& base, string_view path) {
    if (base.empty()) return string{path};
    if (path == "/") {
        // string res = base;
        // res.append("/");
        auto res{base};
        return res.append("/").string();
    }
    auto res = base / string{path};

    return res.string();
    ;
}

template <typename Body, typename Allocator>
http::message_generator handleRequest(fs::path root, http::request<Body, http::basic_fields<Allocator>>&& req) {
    // BAD REQUEST
    const auto badReq = [&req](string_view why) {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = string{why};
        res.prepare_payload();
        return res;
    };

    // NOT FOUND
    const auto notFound = [&req](string_view target) {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "This resource " + string{target} + " was not found";
        res.prepare_payload();
        return res;
    };

    // SERVER ERROR
    const auto serverError = [&req](string_view what) {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: " + string{what};
        res.prepare_payload();
        return res;
    };

    // BAD REQUEST
    cout << formatMsg(req.method_string(), req.target());
    if (req.method() != http::verb::get && req.method() != http::verb::head) return badReq("Unknown HTTP method");

    // BAD REQUEST
    if (req.target().empty() || req.target()[0] != '/' || req.target().find("..") != string_view::npos)
        return badReq("Illegal request target");

    // RESOLVE PATH
    string path = pathCat(root, req.target());
    if (req.target().back() == '/') path.append("index.html");

    beast::error_code ec;
    http::file_body::value_type body;
    body.open(path.c_str(), beast::file_mode::scan, ec);

    // NOT FOUND
    if (ec == beast::errc::no_such_file_or_directory) return notFound(req.target());

    // SERVER ERROR
    if (ec) {
        return serverError(ec.message());
    }

    // CACHE BODY SIZE
    const auto bodySize = body.size();

    // HEAD
    if (req.method() == http::verb::head) {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, resolveMIMEType(path));
        res.content_length(bodySize);
        res.keep_alive(req.keep_alive());
        return res;
    }

    // GET
    http::response<http::file_body> res{std::piecewise_construct, std::make_tuple(std::move(body)),
                                        std::make_tuple(http::status::ok, req.version())};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, resolveMIMEType(path));
    res.content_length(bodySize);
    res.keep_alive(req.keep_alive());
    return res;
}

class Session : public std::enable_shared_from_this<Session> {
   public:
    Session(tcp::socket&& sock, std::shared_ptr<const fs::path>& root) : stream{std::move(sock)}, root{root} {}

    void run() {
        beast::net::dispatch(stream.get_executor(), beast::bind_front_handler(&Session::read, shared_from_this()));
    }

   private:
    void read() {
        parser.emplace();

        parser->body_limit(10000);
        stream.expires_after(std::chrono::seconds{30});

        http::async_read(stream, buffer, *parser, beast::bind_front_handler(&Session::onRead, shared_from_this()));
    }

    void onRead(const beast::error_code& ec, size_t) {
        if (ec == http::error::end_of_stream) return close();

        if (ec) {
            cerr << "read " << ec.message() << endl;
            return;
        }

        queueWrite(handleRequest(*root, parser->release()));

        if (resQueue.size() < queueLimit) read();
    }

    void queueWrite(http::message_generator res) {
        resQueue.push_back(std::move(res));

        if (resQueue.size() == 1) write();
    }

    bool write() {
        bool const wasFull = resQueue.size() == queueLimit;

        if (!resQueue.empty()) {
            http::message_generator msg = std::move(resQueue.front());
            resQueue.erase(resQueue.begin());

            bool keepAlive = msg.keep_alive();

            beast::async_write(stream, std::move(msg),
                               beast::bind_front_handler(&Session::onWrite, shared_from_this(), keepAlive));
        }

        return wasFull;
    }

    void onWrite(bool keepAlive, beast::error_code ec, size_t) {
        if (ec) {
            cerr << formatMsg("write", ec.message());
            return;
        }

        if (!keepAlive) {
            return close();
        }

        if (write()) {
            write();
        }
    }

    void close() {
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_send, ec);
    }

    beast::flat_buffer buffer;
    beast::tcp_stream stream;
    std::shared_ptr<const fs::path> root;
    static constexpr size_t queueLimit = 8;
    vector<http::message_generator> resQueue;
    std::optional<http::request_parser<http::string_body>> parser;
};

class Listener : public std::enable_shared_from_this<Listener> {
   public:
    Listener(asio::io_context& context, tcp::endpoint endpoint, const std::shared_ptr<const fs::path>& root)
        : context{context}, acceptor{asio::make_strand(context)}, root{root} {
        beast::error_code ec;

        acceptor.open(endpoint.protocol(), ec);
        if (ec) {
            cerr << formatMsg("open", ec.what());
            return;
        }

        acceptor.set_option(asio::socket_base::reuse_address(true), ec);
        if (ec) {
            cerr << formatMsg("set_option", ec.what());
            return;
        }

        acceptor.bind(endpoint, ec);
        if (ec) {
            cerr << formatMsg("bind", ec.what());
            return;
        }

        acceptor.listen(asio::socket_base::max_connections, ec);
        if (ec) {
            cerr << formatMsg("listen", ec.what());
            return;
        }
    }

    void run() {
        // asio::dispatch(acceptor.get_executor(), beast::bind_front_handler(&Listener::accept,
        // this->shared_from_this()));
        accept();
    }

   private:
    void accept() {
        acceptor.async_accept(asio::make_strand(context),
                              beast::bind_front_handler(&Listener::onAccept, shared_from_this()));
    }

    void onAccept(beast::error_code ec, tcp::socket sock) {
        if (ec) {
            cerr << formatMsg("accept", ec.what());
            return;

        } else {
            std::make_shared<Session>(std::move(sock), root)->run();
        }

        accept();
    }

    asio::io_context& context;
    tcp::acceptor acceptor;
    std::shared_ptr<const fs::path> root;
};

int main(int argc, char* argv[]) {
    bool isHelp{};
    using namespace op;

    options_description description;
    description.add_options()("help,h", bool_switch(&isHelp), "Display help message")(
        "root,r", value<string>(), "Root directory")("port,p", value<unsigned short>()->default_value(8080),
                                                     "Port used")("threads,t", value<int>()->default_value(2),
                                                                  "Threads used");

    command_line_parser parser{argc, argv};
    parser.options(description);

    variables_map vm;

    try {
        auto parserRes = parser.run();
        store(parserRes, vm);
        notify(vm);
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    if (isHelp) {
        cout << description;
        return 0;
    }

    if (vm["root"].empty()) {
        cerr << "You must provide a root directory" << endl;
        return -1;
    }

    const auto rootDir = vm["root"].as<string>();
    const auto port = vm["port"].as<unsigned short>();
    const auto threads = vm["threads"].as<int>();

    asio::io_context context{threads};
    auto host = asio::ip::address_v4();
    boost::asio::signal_set signals(context, SIGINT, SIGTERM);
    signals.async_wait([&](const beast::error_code&, int) { context.stop(); });

    try {
        const auto root = std::make_shared<const fs::path>(rootDir);
        if (!fs::is_directory(*root)) {
            cerr << "Error path provided is not a directory" << endl;
            return -1;
        }

        cout << "Listening on " << host.to_string() << ":" << port << " Using " << threads << " threads."
             << "\nServing directory " << *root << endl;
        std::make_shared<Listener>(context, tcp::endpoint{host, port}, root)->run();

        vector<future<void>> futures;
        std::generate_n(std::back_inserter(futures), threads,
                        [&] { return std::async(std::launch::async, [&] { context.run(); }); });

        for (auto& future : futures) {
            future.get();
        }

    } catch (const beast::system_error& se) {
        if (se.code().value() == 4) {
            cout << "Exiting..." << endl;
            return 0;
        }
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
    return 0;
}
