#include <iostream>
#include "stdmove.h"


int main() {
    std::string a = "hello";
    std::string b = roman::move(a);

    std::cout << "a: " << a << "\nb: " << b;
}
