#include <iostream>

template <typename T>
class WeakPtr;

template <typename T>
struct ControlBlock
{
    T* ptr;
    size_t strong_count;
    size_t weak_count;
};

template <typename T>
class SharedPtr
{
public:
    SharedPtr() : ptr(nullptr), ctrl(nullptr) {}
    SharedPtr(T* p) : ptr(p)
    {
        ctrl = new ControlBlock<T>(ptr, 1, 0);
    }

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), ctrl(other.ctrl)
    {
        std::cout << "called copy constructor\n";
        ctrl->strong_count++;
    }

    SharedPtr(SharedPtr&& other) : ptr(other.ptr), ctrl(other.ctrl)
    {
        std::cout << "called move constructor\n";
        other.ptr = nullptr;
        other.ctrl = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& other)
    {
        std::cout << "called copy operator=\n";
        release();
        ptr = other.ptr;
        ctrl = other.ctrl;
        ctrl->strong_count++;
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other)
    {
        std::cout << "called move operator=\n";
        release();
        ptr = other.ptr;
        ctrl = other.ctrl;
        other.ptr = nullptr;
        other.ctrl = nullptr;
        return *this;
    }

    int use_count() { return ctrl ? ctrl->strong_count : 0; }

    bool operator==(T* ptr)
    {
        return this->ptr == ptr;
    }

    void release()
    {
        if (ctrl)
            {
            ctrl->strong_count--;
            if (ctrl->strong_count == 0)
            {
                delete ptr;
                delete ctrl;
            }
        }
        ptr = nullptr;
        ctrl = nullptr;
    }

    ~SharedPtr()
    {
        release();
    }

    SharedPtr(ControlBlock<T>* existingCtrl) //для lock()
        : ctrl(existingCtrl), ptr(existingCtrl->ptr)
    {
        if (ctrl)
            ctrl->strong_count++;   // мы создали новый shared_ptr
    }
private:
    T* ptr;
    ControlBlock<T>* ctrl;
    friend class WeakPtr<T>;
};

/*
 еще:
копирующий оператор присваивания от SharedPtr и WeakPtr
перемещающий оператор присваивания от WeakPtr
*/

template <typename T>
class WeakPtr
{
public:
    WeakPtr() : ctrl(nullptr) {}

    WeakPtr(const SharedPtr<T>& sp) : ctrl(sp.ctrl)
    {
        ctrl->weak_count++;
    }

    WeakPtr& operator=(const SharedPtr<int>& other)
    {
        this->ctrl = other.ctrl;
        this->ctrl->weak_count++;
        return *this;
    }

    WeakPtr(const WeakPtr& other) : ctrl(other.ctrl)
    {
        ctrl->weak_count++;
    }

    WeakPtr& operator=(const WeakPtr& other)
    {
        release();

        this->ctrl = other.ctrl;
        this->ctrl->weak_count++;
        return *this;
    }

    WeakPtr(WeakPtr&& other) : ctrl(other.ctrl)
    {
        other.ctrl = nullptr;
    }

    WeakPtr& operator=(WeakPtr&& other)
    {
        release();

        this->ctrl = other.ctrl;

        other.ctrl = nullptr;
        return *this;
    }

    SharedPtr<T> lock() const
    {
        if (this->ctrl != nullptr && this->ctrl->strong_count > 0)
        {
            return SharedPtr<T>(ctrl);
        }
        return SharedPtr<T>();
    }

    ~WeakPtr()
    {
        release();
    }
private:
    ControlBlock<T>* ctrl;

    void release()
    {
        if (ctrl)
        {
            ctrl->weak_count--;
            if (ctrl->weak_count == 0 && ctrl->strong_count ==0)
            {
                delete ctrl;
            }
        }
    }
};