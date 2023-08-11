#include <boost/beast.hpp>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>
#include <string>

namespace asio = boost::asio;
namespace sys = boost::system;
namespace fs = std::filesystem;
namespace beast = boost::beast;
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

string pathCat(fs::path base, string_view path) {
    if (base.empty()) return string{path};
    if (path == "/") {
        string res = base;
        res.append("/");
        return res;
    }
    string res = string{base} + string{path};

    return res;
    ;
}

template <typename Body, typename Allocator, typename Send>
void handleRequest(fs::path root, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {
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
        res.body() = "An error occured: " + string{what};
        res.prepare_payload();
        return res;
    };

    // BAD REQUEST
    cout << "req.method(): " << req.method() << endl;
    cout << "req.target(): " << req.target() << endl;
    if (req.method() != http::verb::get && req.method() != http::verb::head) return send(badReq("Unknown HTTP method"));

    // BAD REQUEST
    if (req.target().empty() || req.target()[0] != '/' || req.target().find("..") != string_view::npos)
        return send(badReq("Illegal request target"));

    // RESOLVE PATH
    string path = pathCat(root, req.target());
    if (req.target().back() == '/') path.append("index.html");

    beast::error_code ec;
    http::file_body::value_type body;
    body.open(path.c_str(), beast::file_mode::scan, ec);

    // NOT FOUND
    if (ec == beast::errc::no_such_file_or_directory) return send(notFound(req.target()));

    // SERVER ERROR
    if (ec) {
        return send(serverError(ec.message()));
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
        return send(std::move(res));
    }

    // GET
    http::response<http::file_body> res{std::piecewise_construct, std::make_tuple(std::move(body)),
                                        std::make_tuple(http::status::ok, req.version())};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, resolveMIMEType(path));
    res.content_length(bodySize);
    res.keep_alive(req.keep_alive());
    return send(std::move(res));
}

void doSession(tcp::socket& sock, fs::path root) {
    bool close = false;
    beast::error_code ec;

    // Required to persist between reads
    beast::flat_buffer buffer;

    const auto send = [&sock, &close, &ec]<bool isRequest, typename Body, typename Fields>(
                          http::message<isRequest, Body, Fields>&& msg) {
        close = msg.need_eof();
        http::serializer<isRequest, Body, Fields> sr{msg};
        http::write(sock, sr, ec);
    };

    while (true) {
        http::request<http::string_body> req;
        http::read(sock, buffer, req, ec);
        if (ec == http::error::end_of_stream) break;
        if (ec) {
            cerr << "read " << ec.message() << endl;
            return;
        }

        handleRequest(root, std::move(req), send);
        if (ec) {
            cerr << "write " << ec.message() << endl;
            return;
        }

        if (close) {
            break;
        }
    }

    sock.shutdown(tcp::socket::shutdown_send, ec);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: beast ROOT_DIR" << endl;
        return -1;
    }

    const int nThreads{3};
    asio::io_context context{nThreads};
    const int port{8080};
    auto host = asio::ip::address_v4();

    try {
        // fs::path root{string{argv[1]}};
        const auto root = std::make_shared<fs::path>(argv[1]);
        if (!fs::is_directory(*root)) {
            cerr << "Error path provided is not a directory" << endl;
            return -1;
        }

        tcp::acceptor acceptor{context, tcp::endpoint{host, port}};
        cout << "Listening on " << host.to_string() << ":" << port << "\nServing directory " << *root << endl;

        while (true) {
            tcp::socket sock{context};
            acceptor.accept(sock);
            doSession(sock, *root);
        }
        // tcp::socket sock{context};
        // acceptor.accept(sock);
        // vector<future<void>> futures;
        // std::generate_n(std::back_inserter(futures), nThreads,
        //                 [&] { return std::async(std::launch::async, [&] { doSession(sock, *root); }); });
        //
        // for (auto& future : futures) {
        //     future.get();
        // }

    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}
