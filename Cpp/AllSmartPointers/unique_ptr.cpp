#include <iostream>
#include <memory>

/*Единственный владелец объекта.
Нельзя копировать.
Можно перемещать.*/

class  my_functor_as_deleter
{
public:
    template <typename T>
    void operator()(T* ptr) const
    {
        std::cout << "functor called";
        delete ptr;
    }
};

template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr
{
public:
    UniquePtr() : ptr(nullptr) {}
    explicit UniquePtr(T* pointer) : ptr(pointer) { std::cout << "called constructor from raw pointer\n"; }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr)
    {
        std::cout << "called move constructor\n";
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other)
    {
        std::cout << "called move operator=\n";
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }

    bool operator==(T* ptr)
    {
        return this->ptr == ptr;
    }

    ~UniquePtr()
    {
        deleter(ptr);
    }

    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
private:
    T* ptr;
    Deleter deleter;
};
