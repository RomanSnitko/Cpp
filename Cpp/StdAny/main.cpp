#include <iostream>
#include <type_traits>

class placeholder //смысл - спрятать конкретный тип
{
public:
    virtual ~placeholder() {}
    virtual placeholder* clone() const = 0; //клонировать
    virtual void* get_ptr() = 0; //вернуть void* на наши данные
    virtual const std::type_info& type() const = 0;
};

template <typename T> //holder<T> знает реальный тип
class holder : public placeholder
{
public:
    holder(const T& v) : value(v) {}

    placeholder* clone() const override
    {
        return new holder(value);
    }

    const std::type_info& type() const override
    {
        return typeid(T);
    }

    void* get_ptr() override
    {
        return &value;
    }
private:
    T value;
};

class myAny //общается только через интерфейс placeholder, не знает ничего про Т.
{
public:
    myAny() : placeholder_(nullptr) {}

    template <typename T>
    myAny(T&& value)
    {
        placeholder_ = new holder<std::decay_t<T>>(std::forward<T>(value));
    }

    myAny(const myAny& other)
    {
        if (other.placeholder_)
            placeholder_ = other.placeholder_->clone(); //для создания новой копии хранимого обьекта того же типа
        else
            placeholder_ = nullptr;
    }

    myAny(myAny&& other)
    {
        placeholder_ = other.placeholder_;
        other.placeholder_ = nullptr;
    }

    ~myAny()
    {
        delete placeholder_;
        placeholder_ = nullptr;
    }

    placeholder* placeholder_;
};

template <typename T>
T& any_cast(myAny& a)
{
    if (!a.placeholder_) throw std::bad_cast();

    if (a.placeholder_->type() != typeid(T)) throw std::bad_cast();

    auto* h = static_cast<holder<T>*>(a.placeholder_);

    return *static_cast<T*>(h->get_ptr());
}


int main()
{

}