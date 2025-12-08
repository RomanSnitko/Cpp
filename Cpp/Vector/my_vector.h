#pragma once
#include <memory>
#include <iostream>
#include <algorithm>
#include <stdexcept>

template<typename T, typename Alloc = std::allocator<T>>
class my_vector {
private:
    T* dynamic_array;
    size_t size_;
    size_t capacity_;
    Alloc alloc_;

public:
    my_vector();
    explicit my_vector(size_t n, const T& value = T());
    my_vector(const my_vector& other);
    my_vector(my_vector&& other) noexcept;

    template<typename InputIterator>
    my_vector(InputIterator first, InputIterator last);

    my_vector(std::initializer_list<T> init);

    ~my_vector();

    my_vector& operator=(const my_vector& other);
    my_vector& operator=(my_vector&& other) noexcept;
    my_vector& operator=(std::initializer_list<T> init);

    T& operator[](size_t i);
    const T& operator[](size_t i) const;

    T& at(size_t i);
    const T& at(size_t i) const;

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;

    T* data() noexcept;
    const T* data() const noexcept;

    void push_back(const T& value);
    void push_back(T&& value);

    template<typename... Args>
    void emplace_back(Args&&... args);

    void pop_back();
    void clear();

    void resize(size_t n);
    void resize(size_t n, const T& value);

    void reserve(size_t n);
    void shrink_to_fit();

    size_t size() const noexcept;
    size_t capacity() const noexcept;
    bool empty() const noexcept;

    void swap(my_vector& other) noexcept;

    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_t count, const T& value);

    template<typename InputIterator>
    iterator insert(const_iterator pos, InputIterator first, InputIterator last);

    iterator insert(const_iterator pos, std::initializer_list<T> init);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    bool operator==(const my_vector& other) const;
    bool operator!=(const my_vector& other) const;
    bool operator<(const my_vector& other) const;
    bool operator>(const my_vector& other) const;
    bool operator<=(const my_vector& other) const;
    bool operator>=(const my_vector& other) const;

private:
    void reallocate(size_t new_capacity);
    void destroy_range(T* first, T* last);
};

#include "my_vector.tpp"