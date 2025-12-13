#pragma once
#include <cstddef>

template<typename T>
class my_list
{
public:
    my_list();
    ~my_list();

    my_list(std::initializer_list<T> init);
    my_list(const my_list& other);
    my_list(my_list&& other);

    my_list& operator=(const my_list& other);
    my_list& operator=(my_list&& other) noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    T& front();
    T& back();

    void push_back(const T& value);
    void push_front(const T& value);
    void pop_back();
    void pop_front();
    void clear();

    class iterator;
    class const_iterator;
    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;
private:
    struct Node;

    Node* sentinel_;
    std::size_t size_;
};

#include "my_list.tpp"