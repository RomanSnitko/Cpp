#ifndef MYSTRING_H
#define MYSTRING_H

#include <cstring>
#include <iostream>
#include <memory>
#include <iterator>
#include <stdexcept>

class myString
{
private:
    char* buffer;
    size_t _size;
    size_t _capacity;
    std::allocator<char> _alloc;

    void reallocate(size_t new_capacity);

public:
    myString();
    explicit myString(const char* array);
    myString(const myString& other);
    myString(myString&& other) noexcept;
    myString(const char* str, const std::allocator<char>& allocator);

    template <typename InputIterator>
    myString(InputIterator first, InputIterator last);

    ~myString();

    myString& operator=(const myString& other);
    myString& operator=(myString&& other) noexcept;
    myString& operator=(const char* str);

    char& operator[](size_t index);
    const char& operator[](size_t index) const;

    bool operator==(const myString& other) const;
    bool operator!=(const myString& other) const;
    bool operator<(const myString& other) const;
    bool operator>(const myString& other) const;
    bool operator<=(const myString& other) const;
    bool operator>=(const myString& other) const;

    myString operator+(const myString& other) const;
    myString& operator+=(const myString& other);
    myString& operator+=(const char* str);

    const char* c_str() const;
    size_t size() const;
    size_t length() const;
    size_t capacity() const;
    bool empty() const;

    void clear();
    void reserve(size_t new_capacity);
    void shrink_to_fit();
    void push_back(char c);
    void pop_back();

    void append(const char* str);
    void append(const myString& str);
    void append(size_t count, char c);

    myString substr(size_t pos, size_t count = std::string::npos) const;

    size_t find(const char* str, size_t pos = 0) const;
    size_t find(char c, size_t pos = 0) const;
    size_t find(const myString& str, size_t pos = 0) const;

    class Iterator;

    Iterator begin();
    Iterator end();
    Iterator begin() const;
    Iterator end() const;
    Iterator cbegin() const;
    Iterator cend() const;

    friend std::ostream& operator<<(std::ostream& os, const myString& str);
    friend std::istream& operator>>(std::istream& is, myString& str);
};

class myString::Iterator
{
private:
    char* current;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = char;
    using difference_type = std::ptrdiff_t;
    using pointer = char*;
    using reference = char&;

    Iterator(char* ptr = nullptr);

    reference operator*() const;
    pointer operator->() const;

    Iterator& operator++();
    Iterator operator++(int);
    Iterator& operator--();
    Iterator operator--(int);

    Iterator operator+(difference_type n) const;
    Iterator operator-(difference_type n) const;
    difference_type operator-(const Iterator& other) const;

    Iterator& operator+=(difference_type n);
    Iterator& operator-=(difference_type n);

    reference operator[](difference_type n) const;

    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
    bool operator<(const Iterator& other) const;
    bool operator>(const Iterator& other) const;
    bool operator<=(const Iterator& other) const;
    bool operator>=(const Iterator& other) const;

    friend Iterator operator+(difference_type n, const Iterator& it);
};

std::ostream& operator<<(std::ostream& os, const myString& str);
std::istream& operator>>(std::istream& is, myString& str);

#endif