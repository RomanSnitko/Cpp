template<typename T, typename Alloc>
my_vector<T, Alloc>::my_vector() : dynamic_array(nullptr), size_(0), capacity_(0) {}

template<typename T, typename Alloc>
my_vector<T, Alloc>::my_vector(size_t n, const T& value) : size_(n), capacity_(n)
{
    dynamic_array = alloc_.allocate(capacity_);
    for (size_t i = 0; i < n; i++)
    {
        std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[i], value);
    }
}

template<typename T, typename Alloc>
my_vector<T, Alloc>::my_vector(const my_vector& other) : size_(other.size_), capacity_(other.capacity_),
alloc_(std::allocator_traits<Alloc>::select_on_container_copy_construction(other.alloc_))
{
    dynamic_array = alloc_.allocate(capacity_);
    for (size_t i = 0; i < size_; i++)
    {
        std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[i], other.dynamic_array[i]);
    }
}

template<typename T, typename Alloc>
my_vector<T, Alloc>::my_vector(my_vector&& other) noexcept
    : dynamic_array(other.dynamic_array), size_(other.size_), capacity_(other.capacity_),
      alloc_(std::move(other.alloc_))
{
    other.dynamic_array = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<typename T, typename Alloc>
template<typename InputIterator>
my_vector<T, Alloc>::my_vector(InputIterator first, InputIterator last)
{
    size_t count = std::distance(first, last);
    size_ = count;
    capacity_ = count;

    dynamic_array = alloc_.allocate(capacity_);

    for (size_t i = 0; first != last; ++first, ++i)
    {
        std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[i], *first);
    }
}

template<typename T, typename Alloc>
my_vector<T, Alloc>::my_vector(std::initializer_list<T> init) : size_(init.size()), capacity_(init.size())
{
    dynamic_array = alloc_.allocate(capacity_);

    size_t i = 0;
    for (const auto& val : init)
    {
        std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[i], val);
        ++i;
    }
}

template<typename T, typename Alloc>
my_vector<T, Alloc>::~my_vector()
{
    destroy_range(dynamic_array, dynamic_array + size_);
    alloc_.deallocate(dynamic_array, capacity_);
}

template<typename T, typename Alloc>
my_vector<T, Alloc>& my_vector<T, Alloc>::operator=(const my_vector& other)
{
    if (this != &other)
    {
        if constexpr (std::allocator_traits<Alloc>::propagate_on_container_copy_assignment::value)
        {
            alloc_ = other.alloc_;
        }

        destroy_range(dynamic_array, dynamic_array + size_);
        alloc_.deallocate(dynamic_array, capacity_);

        size_ = other.size_;
        capacity_ = other.capacity_;
        dynamic_array = alloc_.allocate(capacity_);

        for (size_t i = 0; i < size_; i++)
        {
            std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[i], other.dynamic_array[i]);
        }
    }
    return *this;
}

template<typename T, typename Alloc>
my_vector<T, Alloc>& my_vector<T, Alloc>::operator=(my_vector&& other) noexcept
{
    if (this != &other)
    {
        destroy_range(dynamic_array, dynamic_array + size_);
        alloc_.deallocate(dynamic_array, capacity_);

        dynamic_array = other.dynamic_array;
        size_ = other.size_;
        capacity_ = other.capacity_;

        if constexpr (std::allocator_traits<Alloc>::propagate_on_container_move_assignment::value)
        {
            alloc_ = std::move(other.alloc_);
        }

        other.dynamic_array = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template<typename T, typename Alloc>
my_vector<T, Alloc>& my_vector<T, Alloc>::operator=(std::initializer_list<T> init)
{
    destroy_range(dynamic_array, dynamic_array + size_);
    alloc_.deallocate(dynamic_array, capacity_);

    size_ = init.size();
    capacity_ = size_;
    dynamic_array = alloc_.allocate(capacity_);

    size_t i = 0;
    for (const auto& val : init)
    {
        std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[i], val);
        ++i;
    }

    return *this;
}

template<typename T, typename Alloc>
T& my_vector<T, Alloc>::operator[](size_t i)
{
    return dynamic_array[i];
}

template<typename T, typename Alloc>
const T& my_vector<T, Alloc>::operator[](size_t i) const
{
    return dynamic_array[i];
}

template<typename T, typename Alloc>
T& my_vector<T, Alloc>::at(size_t i)
{
    if (i >= size_)
        throw std::out_of_range("Index out of range");
    return dynamic_array[i];
}

template<typename T, typename Alloc>
const T& my_vector<T, Alloc>::at(size_t i) const
{
    if (i >= size_)
        throw std::out_of_range("Index out of range");
    return dynamic_array[i];
}

template<typename T, typename Alloc>
T& my_vector<T, Alloc>::front()
{
    return dynamic_array[0];
}

template<typename T, typename Alloc>
const T& my_vector<T, Alloc>::front() const
{
    return dynamic_array[0];
}

template<typename T, typename Alloc>
T& my_vector<T, Alloc>::back()
{
    return dynamic_array[size_ - 1];
}

template<typename T, typename Alloc>
const T& my_vector<T, Alloc>::back() const
{
    return dynamic_array[size_ - 1];
}

template<typename T, typename Alloc>
T* my_vector<T, Alloc>::data() noexcept
{
    return dynamic_array;
}

template<typename T, typename Alloc>
const T* my_vector<T, Alloc>::data() const noexcept
{
    return dynamic_array;
}

template<typename T, typename Alloc>
void my_vector<T, Alloc>::push_back(const T& value)
{
    if (size_ >= capacity_)
    {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[size_], value);
    ++size_;
}

template<typename T, typename Alloc>
void my_vector<T, Alloc>::push_back(T&& value)
{
    if (size_ >= capacity_)
    {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[size_], std::move(value));
    ++size_;
}

template<typename T, typename Alloc>
template<typename... Args>
void my_vector<T, Alloc>::emplace_back(Args&&... args)
{
    if (size_ >= capacity_)
    {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[size_], std::forward<Args>(args)...);
    ++size_;
}

template<typename T, typename Alloc>
void my_vector<T, Alloc>::pop_back()
{
    if (size_ > 0)
    {
        --size_;
        std::allocator_traits<Alloc>::destroy(alloc_, &dynamic_array[size_]);
    }
}

template<typename T, typename Alloc>
void my_vector<T, Alloc>::clear()
{
    destroy_range(dynamic_array, dynamic_array + size_);
    size_ = 0;
}

template<typename T, typename Alloc>
void my_vector<T, Alloc>::resize(size_t n)
{
    resize(n, T());
}

template<typename T, typename Alloc>
void my_vector<T, Alloc>::resize(size_t n, const T& value)
{
    if (n < size_)
    {
        destroy_range(dynamic_array + n, dynamic_array + size_);
        size_ = n;
    }
    else if (n > size_)
    {
        if (n > capacity_)
        {
            reallocate(std::max(n, capacity_ * 2));
        }
        for (size_t i = size_; i < n; i++)
        {
            std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[i], value);
        }
        size_ = n;
    }
}

template<typename T, typename Alloc>
void my_vector<T, Alloc>::reallocate(size_t new_capacity)
{
    T* new_array = alloc_.allocate(new_capacity);

    for (size_t i = 0; i < size_; i++)
    {
        std::allocator_traits<Alloc>::construct(alloc_, &new_array[i], std::move_if_noexcept(dynamic_array[i]));
        std::allocator_traits<Alloc>::destroy(alloc_, &dynamic_array[i]);
    }

    alloc_.deallocate(dynamic_array, capacity_);
    dynamic_array = new_array;
    capacity_ = new_capacity;
}

template<typename T, typename Alloc>
void my_vector<T, Alloc>::destroy_range(T* first, T* last)
{
    for (T* p = first; p != last; ++p)
    {
        std::allocator_traits<Alloc>::destroy(alloc_, p);
    }
}

template<typename T, typename Alloc>
void my_vector<T, Alloc>::reserve(size_t n)
{
    if (n > capacity_)
    {
        reallocate(n);
    }
}

template<typename T, typename Alloc>
void my_vector<T, Alloc>::shrink_to_fit()
{
    if (size_ < capacity_)
    {
        reallocate(size_);
    }
}

template<typename T, typename Alloc>
size_t my_vector<T, Alloc>::size() const noexcept
{
    return size_;
}

template<typename T, typename Alloc>
size_t my_vector<T, Alloc>::capacity() const noexcept
{
    return capacity_;
}

template<typename T, typename Alloc>
bool my_vector<T, Alloc>::empty() const noexcept
{
    return size_ == 0;
}

template<typename T, typename Alloc>
void my_vector<T, Alloc>::swap(my_vector& other) noexcept
{
    std::swap(dynamic_array, other.dynamic_array);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);

    if constexpr (std::allocator_traits<Alloc>::propagate_on_container_swap::value)
    {
        std::swap(alloc_, other.alloc_);
    }
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::iterator my_vector<T, Alloc>::begin() noexcept
{
    return dynamic_array;
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::iterator my_vector<T, Alloc>::end() noexcept
{
    return dynamic_array + size_;
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::const_iterator my_vector<T, Alloc>::begin() const noexcept
{
    return dynamic_array;
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::const_iterator my_vector<T, Alloc>::end() const noexcept
{
    return dynamic_array + size_;
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::const_iterator my_vector<T, Alloc>::cbegin() const noexcept
{
    return dynamic_array;
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::const_iterator my_vector<T, Alloc>::cend() const noexcept
{
    return dynamic_array + size_;
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::reverse_iterator my_vector<T, Alloc>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::reverse_iterator my_vector<T, Alloc>::rend() noexcept
{
    return reverse_iterator(begin());
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::const_reverse_iterator my_vector<T, Alloc>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::const_reverse_iterator my_vector<T, Alloc>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::const_reverse_iterator my_vector<T, Alloc>::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::const_reverse_iterator my_vector<T, Alloc>::crend() const noexcept
{
    return const_reverse_iterator(begin());
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::iterator my_vector<T, Alloc>::insert(const_iterator pos, const T& value)
{
    size_t index = pos - begin();

    if (size_ >= capacity_)
    {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    for (size_t i = size_; i > index; --i)
    {
        std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[i], std::move_if_noexcept(dynamic_array[i - 1]));
        std::allocator_traits<Alloc>::destroy(alloc_, &dynamic_array[i - 1]);
    }

    std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[index], value);
    ++size_;

    return dynamic_array + index;
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::iterator my_vector<T, Alloc>::insert(const_iterator pos, T&& value)
{
    size_t index = pos - begin();

    if (size_ >= capacity_)
    {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    for (size_t i = size_; i > index; --i)
    {
        std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[i], std::move_if_noexcept(dynamic_array[i - 1]));
        std::allocator_traits<Alloc>::destroy(alloc_, &dynamic_array[i - 1]);
    }

    std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[index], std::move(value));
    ++size_;

    return dynamic_array + index;
}

template<typename T, typename Alloc>
typename my_vector<T, Alloc>::iterator my_vector<T, Alloc>::erase(const_iterator pos)
{
    size_t index = pos - begin();

    std::allocator_traits<Alloc>::destroy(alloc_, &dynamic_array[index]);

    for (size_t i = index; i < size_ - 1; ++i)
    {
        std::allocator_traits<Alloc>::construct(alloc_, &dynamic_array[i], std::move_if_noexcept(dynamic_array[i + 1]));
        std::allocator_traits<Alloc>::destroy(alloc_, &dynamic_array[i + 1]);
    }

    --size_;
    return dynamic_array + index;
}

template<typename T, typename Alloc>
bool my_vector<T, Alloc>::operator==(const my_vector& other) const
{
    if (size_ != other.size_) return false;
    for (size_t i = 0; i < size_; ++i)
    {
        if (dynamic_array[i] != other.dynamic_array[i]) return false;
    }
    return true;
}

template<typename T, typename Alloc>
bool my_vector<T, Alloc>::operator!=(const my_vector& other) const
{
    return !(*this == other);
}

template<typename T, typename Alloc>
bool my_vector<T, Alloc>::operator<(const my_vector& other) const
{
    size_t min_size = std::min(size_, other.size_);
    for (size_t i = 0; i < min_size; ++i)
    {
        if (dynamic_array[i] != other.dynamic_array[i])
            return dynamic_array[i] < other.dynamic_array[i];
    }
    return size_ < other.size_;
}

template<typename T, typename Alloc>
bool my_vector<T, Alloc>::operator>(const my_vector& other) const
{
    return other < *this;
}

template<typename T, typename Alloc>
bool my_vector<T, Alloc>::operator<=(const my_vector& other) const
{
    return !(other < *this);
}

template<typename T, typename Alloc>
bool my_vector<T, Alloc>::operator>=(const my_vector& other) const
{
    return !(*this < other);
}