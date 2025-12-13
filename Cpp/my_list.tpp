#include "my_list.h"

template <typename T>
struct my_list<T>::Node
{
    T value;
    Node* prev;
    Node* next;

    Node(const T& v) : prev(nullptr), next(nullptr), value(v) {}
};

template<typename T>
my_list<T>::my_list() : size_(0)
{
    sentinel_ = new Node({});
    sentinel_->next = sentinel_;
    sentinel_->prev = sentinel_;
}

template <typename T>
my_list<T>::~my_list()
{
    clear();
    delete sentinel_;
}

template <typename T>
my_list<T>::my_list(const my_list& other) : my_list()
{
    for (const auto& value : other) {
        this->push_back(value);
    }
}

template <typename T>
my_list<T>::my_list(my_list&& other) : sentinel_(other.sentinel_), size_(other.size_)
{
    other.sentinel_ = new Node(T{});
    other.sentinel_->next = other.sentinel_;
    other.sentinel_->prev = other.sentinel_;
    other.size_ = 0;
}

template <typename T>
my_list<T>::my_list(std::initializer_list<T> init) : my_list()
{
    for (const auto& value : init) {
        push_back(value);
    }
}

template <typename T>
my_list<T>& my_list<T>::operator=(const my_list& other)
{
    for (const auto& value : other)
    {
        this->push_back(value);
    }
    return *this;
}

template <typename T>
my_list<T>& my_list<T>::operator=(my_list&& other) noexcept
{
    sentinel_ = other.sentinel_;
    size_ = other.size_;

    other.sentinel_ = new Node(T({}));
    other.sentinel_->prev = other.sentinel_;
    other.sentinel_->next = other.sentinel_;
    other.size_ = 0;

    return *this;
}

template<typename T>
void my_list<T>::push_back(const T &value)
{
    Node* node = new Node(value);
    node->prev = sentinel_->prev;
    node->next = sentinel_;

    sentinel_->prev->next = node;
    sentinel_->prev = node;

    ++size_;
}

template<typename T>
void my_list<T>::pop_back()
{
    Node* node = sentinel_->prev;
    sentinel_->prev->prev->next = sentinel_;
    sentinel_->prev = sentinel_->prev->prev;

    delete node;
    --size_;
}

template<typename T>
void my_list<T>::push_front(const T &value)
{
    Node* node = new Node(value);
    node->next = sentinel_->next;
    node->prev = sentinel_;

    sentinel_->next->prev = node;
    sentinel_->next = node;

    ++size_;
}

template<typename T>
void my_list<T>::pop_front()
{
    Node* node = sentinel_->next;
    sentinel_->next->next->prev = sentinel_;
    sentinel_->next = sentinel_->next->next;

    delete node;
    --size_;
}

template<typename T>
void my_list<T>::clear()
{
    while (!empty()) {
        pop_front();
    }
}

template <typename T>
std::size_t my_list<T>::size() const noexcept
{
    return size_;
}

template <typename T>
class my_list<T>::iterator
{
    Node* node_;
public:
    iterator(Node* n) : node_(n) {}

    T& operator*() const { return node_->value; }

    iterator& operator++() {
        node_ = node_->next;
        return *this;
    }

    iterator operator++(int) {
        iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    iterator& operator--() {
        node_ = node_->prev;
        return *this;
    }

    iterator operator--(int) {
        iterator tmp = *this;
        --(*this);
        return tmp;
    }

    bool operator==(const iterator& other) const { return node_ == other.node_; }
    bool operator!=(const iterator& other) const { return node_ != other.node_; }

    friend class my_list;
};

template <typename T>
class my_list<T>::const_iterator {
    const Node* node_;
public:
    const_iterator(const Node* n) : node_(n) {}

    const T& operator*() const { return node_->value; }

    const_iterator& operator++() {
        node_ = node_->next;
        return *this;
    }

    const_iterator operator++(int) {
        const_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    const_iterator& operator--() {
        node_ = node_->prev;
        return *this;
    }

    const_iterator operator--(int) {
        const_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    bool operator==(const const_iterator& other) const { return node_ == other.node_; }
    bool operator!=(const const_iterator& other) const { return node_ != other.node_; }
};

template<typename T>
typename my_list<T>::iterator my_list<T>::begin() {
    return iterator(sentinel_->next);
}

template<typename T>
typename my_list<T>::iterator my_list<T>::end() {
    return iterator(sentinel_);
}

template <typename T>
typename my_list<T>::const_iterator my_list<T>::begin() const {
    return const_iterator(sentinel_->next);
}

template <typename T>
typename my_list<T>::const_iterator my_list<T>::end() const {
    return const_iterator(sentinel_);
}

template<typename T>
T& my_list<T>::front()
{
    return sentinel_->next->value;
}

template<typename T>
T& my_list<T>::back()
{
    return sentinel_->prev->value;
}

template<typename T>
bool my_list<T>::empty() const noexcept {
    return size_ == 0;
}

