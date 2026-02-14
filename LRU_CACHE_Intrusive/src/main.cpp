#include <iostream>
#include "lru/lru_cache.hpp"

int main() 
{
    lru::LRUCache<int, std::string> cache(2);

    cache.put(1, "A");
    cache.put(2, "B");

    std::cout << "key 1: " << cache.get(1).value_or("missing") << "\n";

    cache.put(3, "C");

    std::cout << "key 2: " << cache.get(2).value_or("missing") << "\n"; // missing
    std::cout << "key 3: " << cache.get(3).value_or("missing") << "\n"; // C
}
