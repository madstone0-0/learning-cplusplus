#include <boost/asio.hpp>
#include <chrono>
#include <iostream>

namespace asio = boost::asio;
using std::cout, std::endl;
/* Steady Timers */
asio::steady_timer make_timer(asio::io_context& context) {
    return asio::steady_timer{context, std::chrono::steady_clock::now() + std::chrono::seconds{3}};
}

int main() {
    asio::io_context context;

    auto timer1 = make_timer(context);
    cout << "Entering steady_timer::wait\n";
    timer1.wait();
    cout << "Exited steady_timer::wait\n";

    auto timer2 = make_timer(context);
    cout << "Entering steady_timer::async_wait\n";
    timer2.async_wait([](const boost::system::error_code error) {
        if (!error) cout << "<<callback function>>\n";
    });
    cout << "Exited steady_timer::async_wait\n";

    cout << "Entering context run\n";
    context.run();
    cout << "Exited context run\n";

    return 0;
}

/* Steady Timers */
