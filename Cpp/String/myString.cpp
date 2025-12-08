#include "myString.h"

myString::myString() : buffer(nullptr), _size(0), _capacity(0) {}

myString::myString(const char* array) : _size(std::strlen(array)), _capacity(_size)
{
    buffer = _alloc.allocate(_capacity + 1);
    for (size_t i = 0; i < _size; ++i)
    {
        std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + i, array[i]);
    }
    std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + _size, '\0');
}

myString::myString(const myString& other) : _size(other._size), _capacity(other._capacity)
{
    buffer = _alloc.allocate(_capacity + 1);
    for (size_t i = 0; i < _size; ++i)
    {
        std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + i, other.buffer[i]);
    }
    std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + _size, '\0');
}

myString::myString(myString&& other) noexcept :
    buffer(other.buffer), _size(other._size), _capacity(other._capacity), _alloc(std::move(other._alloc))
{
    other.buffer = nullptr;
    other._size = 0;
    other._capacity = 0;
}

myString::myString(const char* str, const std::allocator<char>& allocator) :
    _alloc(allocator), _size(std::strlen(str)), _capacity(_size)
{
    buffer = _alloc.allocate(_capacity + 1);
    for (size_t i = 0; i < _size; ++i)
    {
        std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + i, str[i]);
    }
    std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + _size, '\0');
}

template <typename InputIterator>
myString::myString(InputIterator first, InputIterator last)
{
    _size = std::distance(first, last);
    _capacity = _size;
    buffer = _alloc.allocate(_capacity + 1);

    char* ptr = buffer;
    for (InputIterator it = first; it != last; ++it)
    {
        std::allocator_traits<std::allocator<char>>::construct(_alloc, ptr, *it);
        ++ptr;
    }
    std::allocator_traits<std::allocator<char>>::construct(_alloc, ptr, '\0');
}

myString::~myString()
{
    if (buffer)
    {
        for (size_t i = 0; i < _size; ++i)
        {
            std::allocator_traits<std::allocator<char>>::destroy(_alloc, buffer + i);
        }
        _alloc.deallocate(buffer, _capacity + 1);
    }
}

myString& myString::operator=(const myString& other)
{
    if (this == &other) return *this;

    if (buffer)
    {
        for (size_t i = 0; i < _size; ++i)
        {
            std::allocator_traits<std::allocator<char>>::destroy(_alloc, buffer + i);
        }
        _alloc.deallocate(buffer, _capacity + 1);
    }

    _size = other._size;
    _capacity = other._capacity;
    buffer = _alloc.allocate(_capacity + 1);

    for (size_t i = 0; i < _size; ++i)
    {
        std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + i, other.buffer[i]);
    }
    std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + _size, '\0');

    return *this;
}

myString& myString::operator=(myString&& other) noexcept
{
    if (this == &other) return *this;

    if (buffer)
    {
        for (size_t i = 0; i < _size; ++i)
        {
            std::allocator_traits<std::allocator<char>>::destroy(_alloc, buffer + i);
        }
        _alloc.deallocate(buffer, _capacity + 1);
    }

    buffer = other.buffer;
    _size = other._size;
    _capacity = other._capacity;
    _alloc = std::move(other._alloc);

    other.buffer = nullptr;
    other._size = 0;
    other._capacity = 0;

    return *this;
}

myString& myString::operator=(const char* str)
{
    size_t new_size = std::strlen(str);

    if (new_size > _capacity)
    {
        if (buffer)
        {
            for (size_t i = 0; i < _size; ++i)
            {
                std::allocator_traits<std::allocator<char>>::destroy(_alloc, buffer + i);
            }
            _alloc.deallocate(buffer, _capacity + 1);
        }

        _capacity = new_size;
        buffer = _alloc.allocate(_capacity + 1);
    }

    _size = new_size;
    for (size_t i = 0; i < _size; ++i)
    {
        std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + i, str[i]);
    }
    std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + _size, '\0');

    return *this;
}

char& myString::operator[](size_t index)
{
    if (index >= _size) throw std::out_of_range("Index out of range");
    return buffer[index];
}

const char& myString::operator[](size_t index) const
{
    if (index >= _size) throw std::out_of_range("Index out of range");
    return buffer[index];
}

bool myString::operator==(const myString& other) const
{
    if (_size != other._size) return false;
    for (size_t i = 0; i < _size; ++i)
    {
        if (buffer[i] != other.buffer[i]) return false;
    }
    return true;
}

bool myString::operator!=(const myString& other) const
{
    return !(*this == other);
}

bool myString::operator<(const myString& other) const
{
    size_t min_size = _size < other._size ? _size : other._size;
    for (size_t i = 0; i < min_size; ++i)
    {
        if (buffer[i] != other.buffer[i])
            return buffer[i] < other.buffer[i];
    }
    return _size < other._size;
}

bool myString::operator>(const myString& other) const
{
    return other < *this;
}

bool myString::operator<=(const myString& other) const
{
    return !(other < *this);
}

bool myString::operator>=(const myString& other) const
{
    return !(*this < other);
}

myString myString::operator+(const myString& other) const
{
    myString result;
    result.reserve(_size + other._size);

    for (size_t i = 0; i < _size; ++i)
    {
        result.push_back(buffer[i]);
    }

    for (size_t i = 0; i < other._size; ++i)
    {
        result.push_back(other.buffer[i]);
    }

    return result;
}

myString& myString::operator+=(const myString& other)
{
    reserve(_size + other._size);

    for (size_t i = 0; i < other._size; ++i)
    {
        push_back(other.buffer[i]);
    }

    return *this;
}

myString& myString::operator+=(const char* str)
{
    size_t len = std::strlen(str);
    reserve(_size + len);

    for (size_t i = 0; i < len; ++i)
    {
        push_back(str[i]);
    }

    return *this;
}

const char* myString::c_str() const
{
    return buffer ? buffer : "";
}

size_t myString::size() const { return _size; }
size_t myString::length() const { return _size; }
size_t myString::capacity() const { return _capacity; }

bool myString::empty() const { return _size == 0; }

void myString::clear()
{
    for (size_t i = 0; i < _size; ++i)
    {
        std::allocator_traits<std::allocator<char>>::destroy(_alloc, buffer + i);
    }
    _size = 0;
    if (buffer)
    {
        std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer, '\0');
    }
}

void myString::reallocate(size_t new_capacity)
{
    if (new_capacity <= _capacity) return;

    char* new_buffer = _alloc.allocate(new_capacity + 1);

    for (size_t i = 0; i < _size; ++i)
    {
        std::allocator_traits<std::allocator<char>>::construct(_alloc, new_buffer + i, buffer[i]);
    }
    std::allocator_traits<std::allocator<char>>::construct(_alloc, new_buffer + _size, '\0');

    if (buffer)
    {
        for (size_t i = 0; i < _size; ++i)
        {
            std::allocator_traits<std::allocator<char>>::destroy(_alloc, buffer + i);
        }
        _alloc.deallocate(buffer, _capacity + 1);
    }

    buffer = new_buffer;
    _capacity = new_capacity;
}

void myString::reserve(size_t new_capacity)
{
    if (new_capacity > _capacity)
    {
        reallocate(new_capacity);
    }
}

void myString::shrink_to_fit()
{
    if (_size < _capacity)
    {
        reallocate(_size);
    }
}

void myString::push_back(char c)
{
    if (_size == _capacity)
    {
        reallocate(_capacity == 0 ? 1 : _capacity * 2);
    }

    std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + _size, c);
    ++_size;
    std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + _size, '\0');
}

void myString::pop_back()
{
    if (_size == 0) return;

    --_size;
    std::allocator_traits<std::allocator<char>>::destroy(_alloc, buffer + _size);
    std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + _size, '\0');
}

void myString::append(const char* str)
{
    size_t len = std::strlen(str);
    reserve(_size + len);

    for (size_t i = 0; i < len; ++i)
    {
        std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + _size + i, str[i]);
    }
    _size += len;
    std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + _size, '\0');
}

void myString::append(const myString& str)
{
    append(str.c_str());
}

void myString::append(size_t count, char c)
{
    reserve(_size + count);

    for (size_t i = 0; i < count; ++i)
    {
        std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + _size + i, c);
    }
    _size += count;
    std::allocator_traits<std::allocator<char>>::construct(_alloc, buffer + _size, '\0');
}

myString myString::substr(size_t pos, size_t count) const
{
    if (pos > _size) throw std::out_of_range("Position out of range");

    size_t actual_count = count;
    if (count == std::string::npos || pos + count > _size)
    {
        actual_count = _size - pos;
    }

    myString result;
    result.reserve(actual_count);

    for (size_t i = 0; i < actual_count; ++i)
    {
        result.push_back(buffer[pos + i]);
    }

    return result;
}

size_t myString::find(const char* str, size_t pos) const
{
    size_t len = std::strlen(str);
    if (len == 0) return pos <= _size ? pos : std::string::npos;

    for (size_t i = pos; i <= _size - len; ++i)
    {
        bool found = true;
        for (size_t j = 0; j < len; ++j)
        {
            if (buffer[i + j] != str[j])
            {
                found = false;
                break;
            }
        }
        if (found) return i;
    }

    return std::string::npos;
}

size_t myString::find(char c, size_t pos) const
{
    for (size_t i = pos; i < _size; ++i)
    {
        if (buffer[i] == c) return i;
    }
    return std::string::npos;
}

size_t myString::find(const myString& str, size_t pos) const
{
    return find(str.c_str(), pos);
}

myString::Iterator myString::begin() { return Iterator(buffer); }
myString::Iterator myString::end() { return Iterator(buffer + _size); }
myString::Iterator myString::begin() const { return Iterator(buffer); }
myString::Iterator myString::end() const { return Iterator(buffer + _size); }
myString::Iterator myString::cbegin() const { return Iterator(buffer); }
myString::Iterator myString::cend() const { return Iterator(buffer + _size); }

myString::Iterator::Iterator(char* ptr) : current(ptr) {}

char& myString::Iterator::operator*() const { return *current; }
char* myString::Iterator::operator->() const { return current; }

myString::Iterator& myString::Iterator::operator++()
{
    ++current;
    return *this;
}

myString::Iterator myString::Iterator::operator++(int)
{
    Iterator temp = *this;
    ++current;
    return temp;
}

myString::Iterator& myString::Iterator::operator--()
{
    --current;
    return *this;
}

myString::Iterator myString::Iterator::operator--(int)
{
    Iterator temp = *this;
    --current;
    return temp;
}

myString::Iterator myString::Iterator::operator+(difference_type n) const
{
    return Iterator(current + n);
}

myString::Iterator myString::Iterator::operator-(difference_type n) const
{
    return Iterator(current - n);
}

myString::Iterator::difference_type myString::Iterator::operator-(const Iterator& other) const
{
    return current - other.current;
}

myString::Iterator& myString::Iterator::operator+=(difference_type n)
{
    current += n;
    return *this;
}

myString::Iterator& myString::Iterator::operator-=(difference_type n)
{
    current -= n;
    return *this;
}

char& myString::Iterator::operator[](difference_type n) const
{
    return current[n];
}

bool myString::Iterator::operator==(const Iterator& other) const
{
    return current == other.current;
}

bool myString::Iterator::operator!=(const Iterator& other) const
{
    return current != other.current;
}

bool myString::Iterator::operator<(const Iterator& other) const
{
    return current < other.current;
}

bool myString::Iterator::operator>(const Iterator& other) const
{
    return current > other.current;
}

bool myString::Iterator::operator<=(const Iterator& other) const
{
    return current <= other.current;
}

bool myString::Iterator::operator>=(const Iterator& other) const
{
    return current >= other.current;
}

myString::Iterator operator+(myString::Iterator::difference_type n, const myString::Iterator& it)
{
    return it + n;
}

std::ostream& operator<<(std::ostream& os, const myString& str)
{
    return os << (str.buffer ? str.buffer : "");
}

std::istream& operator>>(std::istream& is, myString& str)
{
    str.clear();
    char c;
    while (is.get(c) && !std::isspace(c))
    {
        str.push_back(c);
    }
    return is;
}