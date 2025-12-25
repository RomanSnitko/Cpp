#include "my_deque.h"

template<typename T, typename Alloc>
my_deque<T, Alloc>::my_deque()
    : map(nullptr), map_size(8), start(), finish()
{
    map = _map_alloc.allocate(map_size);
    for (size_t i = 0; i < map_size; ++i)
    {
        map[i] = nullptr;
    }

    size_t central_index = map_size / 2;
    map[central_index] = _data_alloc.allocate(DEQUE_BLOCK_SIZE);

    start.set_node(&map[central_index]);
    start.cur = start.first;
    finish = start;
}

template <typename T, typename Alloc>
my_deque<T, Alloc>::my_deque(const my_deque& other)
    : _data_alloc(std::allocator_traits<Alloc>::select_on_container_copy_construction(other._data_alloc)),
      _map_alloc(_data_alloc), map_size(other.map_size), start(), finish()
{
    map = _map_alloc.allocate(map_size);
    for (size_t i = 0; i < map_size; ++i)
    {
        map[i] = nullptr;
    }

    size_t mid = map_size / 2;
    map[mid] = _data_alloc.allocate(DEQUE_BLOCK_SIZE);
    start.set_node(&map[mid]);
    start.cur = start.first;
    finish = start;

    for (const auto& value : other) {
        push_back(value);
    }
}

template <typename T, typename Alloc>
my_deque<T, Alloc>::my_deque(my_deque&& other) noexcept
    : _data_alloc(std::move(other._data_alloc)), _map_alloc(std::move(other._map_alloc)),
      map(other.map), map_size(other.map_size), start(other.start), finish(other.finish)
{
    other.map = nullptr;
    other.map_size = 0;
    other.start = Iterator();
    other.finish = Iterator();
}

template <typename T, typename Alloc>
my_deque<T, Alloc>& my_deque<T, Alloc>::operator=(const my_deque& other)
{
    if (this != &other) {
        //propagate allocator if needed
        if (std::allocator_traits<Alloc>::propagate_on_container_copy_assignment::value) {
            _data_alloc = other._data_alloc;
            _map_alloc = other._map_alloc;
        }

        my_deque tmp(other); // copy and swap idiom
        std::swap(map, tmp.map);
        std::swap(map_size, tmp.map_size);
        std::swap(start, tmp.start);
        std::swap(finish, tmp.finish);
    }
    return *this;
}

template <typename T, typename Alloc>
my_deque<T, Alloc>& my_deque<T, Alloc>::operator=(my_deque&& other) noexcept
{
    if (this != &other) {
        this->~my_deque();

        if (std::allocator_traits<Alloc>::propagate_on_container_move_assignment::value) {
            _data_alloc = std::move(other._data_alloc);
            _map_alloc = std::move(other._map_alloc);
        }

        map = other.map;
        map_size = other.map_size;
        start = other.start;
        finish = other.finish;

        other.map = nullptr;
        other.map_size = 0;
    }
    return *this;
}

template <typename T, typename Alloc>
my_deque<T, Alloc>& my_deque<T, Alloc>::operator=(std::initializer_list<T> init)
{
    my_deque tmp(init); //mega safe
    std::swap(map, tmp.map);
    std::swap(map_size, tmp.map_size);
    std::swap(start, tmp.start);
    std::swap(finish, tmp.finish);
    return *this;
}

template <typename T, typename Alloc>
my_deque<T, Alloc>::~my_deque()
{
    if (!map) return;
    while (!empty())
    {
        pop_back();
    }

    if (start.node)
    {
        _data_alloc.deallocate(*start.node, DEQUE_BLOCK_SIZE);
    }
    _map_alloc.deallocate(map, map_size);
    map = nullptr;
}

template <typename T, typename Alloc>
T& my_deque<T, Alloc>::operator[](size_t index)
{
    return *(start + index);
}

template <typename T, typename Alloc>
T& my_deque<T, Alloc>::at(size_t index)
{
    if (index >= size()) throw std::out_of_range("deq out of range");
    return (*this)[index];
}

template <typename T, typename Alloc>
T& my_deque<T, Alloc>::front()
{
    return *start;
}

template <typename T, typename Alloc>
T& my_deque<T, Alloc>::back()
{
    Iterator tmp = finish;
    --tmp;
    return *tmp;
}

template <typename T, typename Alloc>
size_t my_deque<T, Alloc>::size() const
{
    if (!map || start.cur == nullptr) return 0;
    return static_cast<size_t>(finish - start);
}

template <typename T, typename Alloc>
bool my_deque<T, Alloc>::empty() const
{
    return start == finish;
}

template <typename T, typename Alloc>
void my_deque<T, Alloc>::push_back(const T& value)
{
    std::allocator_traits<Alloc>::construct(_data_alloc, finish.cur, value);

    if (finish.cur == finish.last - 1)
    {
        if (finish.node == &map[map_size - 1])
        {
            reallocate_map();
        }

        if (*(finish.node + 1) == nullptr)
        {
            *(finish.node + 1) = _data_alloc.allocate(DEQUE_BLOCK_SIZE);
        }

        finish.set_node(finish.node + 1);
        finish.cur = finish.first;
    }
    else
    {
        ++finish.cur;
    }
}

template <typename T, typename Alloc>
void my_deque<T, Alloc>::push_front(const T& value) {
    if (start.cur == start.first)
    {
        if (start.node == &map[0])
        {
            reallocate_map();
        }

        if (*(start.node - 1) == nullptr)
        {
            *(start.node - 1) = _data_alloc.allocate(DEQUE_BLOCK_SIZE);
        }
        start.set_node(start.node - 1);
        start.cur = start.last;
    }
    --start.cur;
    std::allocator_traits<Alloc>::construct(_data_alloc, start.cur, value);
}

template <typename T, typename Alloc>
void my_deque<T, Alloc>::pop_back()
{
    if (empty()) return;

    if (finish.cur == finish.first)
    {
        finish.set_node(finish.node - 1);
        finish.cur = finish.last;
    }
    --finish.cur;
    std::allocator_traits<Alloc>::destroy(_data_alloc, finish.cur);
}

template <typename T, typename Alloc>
void my_deque<T, Alloc>::pop_front()
{
    if (empty()) return;

    std::allocator_traits<Alloc>::destroy(_data_alloc, start.cur);
    ++start.cur;

    if (start.cur == start.last)
    {
        start.set_node(start.node + 1);
        start.cur = start.first;
    }
}

template <typename T, typename Alloc>
void my_deque<T, Alloc>::reallocate_map()
{
    size_t new_map_size = map_size * 2;
    T** new_map = _map_alloc.allocate(new_map_size);
    for (size_t i = 0; i < new_map_size; ++i)
    {
        new_map[i] = nullptr;
    }

    size_t old_num_nodes = finish.node - start.node + 1;
    size_t new_start_idx = (new_map_size - old_num_nodes) / 2;

    for (size_t i = 0; i < old_num_nodes; ++i)
    {
        new_map[new_start_idx + i] = *(start.node + i);
    }

    size_t start_off = start.cur - start.first;
    size_t finish_off = finish.cur - finish.first;

    _map_alloc.deallocate(map, map_size);
    map = new_map;
    map_size = new_map_size;

    start.set_node(&map[new_start_idx]);
    start.cur = start.first + start_off;
    finish.set_node(&map[new_start_idx + old_num_nodes - 1]);
    finish.cur = finish.first + finish_off;
}

template <typename T, typename Alloc>
typename my_deque<T, Alloc>::Iterator& my_deque<T, Alloc>::Iterator::operator++()
{
    ++cur;
    if (cur == last)
    {
        set_node(node + 1);
        cur = first;
    }
    return *this;
}

template <typename T, typename Alloc>
typename my_deque<T, Alloc>::Iterator my_deque<T, Alloc>::Iterator::operator++(int)
{
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

template <typename T, typename Alloc>
typename my_deque<T, Alloc>::Iterator& my_deque<T, Alloc>::Iterator::operator--()
{
    if (cur == first)
    {
        set_node(node - 1);
        cur = last;
    }
    --cur;
    return *this;
}

template <typename T, typename Alloc>
typename my_deque<T, Alloc>::Iterator my_deque<T, Alloc>::Iterator::operator+(ptrdiff_t n) const
{
    Iterator tmp = *this;
    if (n >= 0)
    {
        size_t offset = static_cast<size_t>(n) + (tmp.cur - tmp.first);
        if (offset < DEQUE_BLOCK_SIZE)
        {
            tmp.cur += n;
        }
        else
        {
            size_t node_offset = offset / DEQUE_BLOCK_SIZE;
            tmp.set_node(tmp.node + node_offset);
            tmp.cur = tmp.first + (offset % DEQUE_BLOCK_SIZE);
        }
    } else {
        ptrdiff_t offset = n - (tmp.first - tmp.cur);
        if (offset >= 0)
        {
            tmp.cur += n;
        }
        else
        {
            ptrdiff_t node_offset = -((-offset - 1) / static_cast<ptrdiff_t>(DEQUE_BLOCK_SIZE)) - 1;
            tmp.set_node(tmp.node + node_offset);
            tmp.cur = tmp.last + (offset % static_cast<ptrdiff_t>(DEQUE_BLOCK_SIZE));
        }
    }
    return tmp;
}

template <typename T, typename Alloc>
ptrdiff_t my_deque<T, Alloc>::Iterator::operator-(const Iterator& other) const
{
    if (!node || !other.node) return 0;
    return ptrdiff_t(DEQUE_BLOCK_SIZE) * (node - other.node) + (cur - first) - (other.cur - other.first);
}

template <typename T, typename Alloc>
T& my_deque<T, Alloc>::Iterator::operator*() const
{
    return *cur;
}

template <typename T, typename Alloc>
bool my_deque<T, Alloc>::Iterator::operator==(const Iterator& other) const
{
    return cur == other.cur;
}

template <typename T, typename Alloc>
void my_deque<T, Alloc>::Iterator::set_node(T** new_node)
{
    node = new_node;
    first = *new_node;
    last = first + DEQUE_BLOCK_SIZE;
}