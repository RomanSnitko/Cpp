#pragma once

#include <memory>
/*
Когда место в текущем блоке заканчивается,дек выделяет новый блок и
записывает указатель на него в «карту».
Если заканчивается место в самой «карте», перевыделяется только
массив указателей (что гораздо дешевле, чем копировать все элементы, как это делает вектор).
 */

static constexpr size_t DEQUE_BLOCK_SIZE = 16;

template <typename T, typename Alloc = std::allocator<T>>
class my_deque
{
public:
    my_deque();
    explicit my_deque(size_t n, const T& value = T());
    my_deque(const my_deque& other);
    my_deque(my_deque&& other);

    my_deque& operator=(const my_deque& other);
    my_deque& operator=(my_deque&& other) noexcept;
    my_deque& operator=(std::initializer_list<T> init);
    ~my_deque();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    T& at(size_t index);
    T& front();
    T& back();

    void push_back(const T& value);
    void push_front(const T& value);
    void pop_back();
    void pop_front();

    size_t size();
    bool empty();

    class Iterator;
private:
    T** map;
    size_t map_size;
    Iterator start;
    Iterator finish;
    Alloc _data_alloc;

    using map_allocator = typename std::allocator_traits<Alloc>::template rebind_alloc<T*>;
    map_allocator _map_alloc;

    void reallocate_map();
    void create_map(size_t num_elements);
};

template <typename T, typename Alloc>
class my_deque<T, Alloc>::Iterator
{
    T* cur;
    T* first;
    T* last;
    T** node;

    friend my_deque;
public:
    Iterator& operator++();
    Iterator& operator--();

    Iterator operator+(ptrdiff_t n) const;
    Iterator operator-(ptrdiff_t n) const;
    ptrdiff_t operator-(const Iterator& other) const;

    T& operator*() const;
    T* operator->() const;

    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;

    void set_node(T** new_node); //for jumping between blocks
};
