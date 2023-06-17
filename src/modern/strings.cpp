#include <cstdio>
#include <iostream>
#include <iterator>
#include <string>

int main() {
    std::string baller = "SHADOW WIZARD MONEY GANG";
    std::cout << baller << std::endl;
    std::cout << "Enter string" << std::endl;
    // std::getline(std::cin, baller);
    // std::cout << "You entered: " << baller << std::endl;
    printf("%s\n", baller.c_str());
    printf("%s\n", baller.substr(3, -6).c_str());

    std::string find = "ler";
    std::string::size_type found = baller.find(find);
    if (found != std::string::npos) {
        printf("%s found at %lu\n", find.c_str(), found);
    } else {
        printf("%s not found\n", find.c_str());
    }

    double obj = 2.4;
    double* obj_ptr = &obj;
    printf("%f\n", *obj_ptr);

    double mydouble = 3.41;
    double& myreference = mydouble;
    printf("%f\n", mydouble);
    myreference = 5.312;
    printf("%f\n", myreference);
    printf("%f\n", mydouble);
    mydouble -= 4.23;
    printf("%f\n", myreference);
    printf("%f\n", mydouble);

    return 0;
}
