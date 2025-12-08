#include "my_vector.h"
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    my_vector<int> v1;

    for (int i = 0; i < 10; ++i) {
        v1.push_back(i);
    }

    std::cout << "Size: " << v1.size() << ", Capacity: " << v1.capacity() << std::endl;

    my_vector<int> v2 = v1;
    std::cout << "Copy constructor: ";
    for (const auto& val : v2) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    my_vector<int> v3 = std::move(v2);
    std::cout << "Move constructor: ";
    for (const auto& val : v3) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    v1.resize(5);
    std::cout << "After resize to 5: ";
    for (const auto& val : v1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    v1.reserve(20);
    std::cout << "After reserve 20, capacity: " << v1.capacity() << std::endl;

    v1.push_back(100);
    v1.emplace_back(200);

    std::cout << "After push_back and emplace_back: ";
    for (const auto& val : v1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    auto it = v1.insert(v1.begin() + 2, 999);
    std::cout << "After insert at position 2: ";
    for (const auto& val : v1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    v1.erase(v1.begin() + 3);
    std::cout << "After erase at position 3: ";
    for (const auto& val : v1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    my_vector<int> v4 = {1, 2, 3, 4, 5};
    std::cout << "Initializer list: ";
    for (const auto& val : v4) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    v4.shrink_to_fit();
    std::cout << "After shrink_to_fit, capacity: " << v4.capacity() << std::endl;

    std::cout << "Front: " << v4.front() << ", Back: " << v4.back() << std::endl;

    my_vector<my_string> strings;
    strings.push_back("Hello");
    strings.emplace_back("World");
    strings.emplace_back("!");

    std::cout << "Strings: ";
    for (const auto& str : strings) {
        std::cout << str << " ";
    }
    std::cout << std::endl;

    return 0;
}