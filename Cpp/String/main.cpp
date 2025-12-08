#include "myString.h"
#include <vector>
#include <algorithm>

int main()
{
    myString str1("hello");
    myString str2("qorld");

    myString str3 = str1 + str2;
    std::cout << str3 << std::endl;

    myString str4(str1);
    std::cout << "copy: " << str4 << std::endl;

    myString str5(std::move(str4));
    std::cout << "move: " << str5 << std::endl;

    str1.append("!!!");
    std::cout << "appended: " << str1 << std::endl;

    std::cout << "size: " << str1.size() << ", capacity: " << str1.capacity() << std::endl;

    str1.reserve(50);
    std::cout << "after reserve: " << str1.capacity() << std::endl;

    for (auto it = str1.begin(); it != str1.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    for (char c : str2)
    {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    myString substr = str3.substr(6, 5);
    std::cout << "substring: " << substr << std::endl;

    std::vector<char> chars = {'T', 'e', 's', 't'};
    myString fromIter(chars.begin(), chars.end());
    std::cout << "from iterator: " << fromIter << std::endl;

    std::allocator<char> customAlloc;
    myString withAlloc("custom allocator", customAlloc);
    std::cout << "with custom allocator: " << withAlloc << std::endl;
}