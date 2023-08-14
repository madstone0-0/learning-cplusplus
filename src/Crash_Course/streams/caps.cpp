#include <boost/algorithm/string/case_conv.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using boost::algorithm::to_upper_copy;

int main() {
    stringstream ss;

    cout << "Enter text:\n";
    cin >> ss.rdbuf();
    cout << to_upper_copy(ss.str()) << endl;

    return 0;
}
