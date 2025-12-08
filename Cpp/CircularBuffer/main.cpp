#include <iostream>
#include <memory>
#include <exception>
#include <stdexcept>

template <typename T>
class CircularBuffer
{
public:
    explicit CircularBuffer(int _capacity) : cap(_capacity), data(std::make_unique<T[]>(_capacity)),
        head(0), tail(0), count(0)
    {
        if (_capacity <= 0) throw std::invalid_argument("capacity should be > 0");
    }

    void push(const T& value)
    {
        data[tail] = value;
        tail = (tail + 1) % cap;
        if (count == cap) { head = (head + 1) % cap; }
        else { count++; }
    }

    T pop()
    {
        if (empty()) throw std::runtime_error("Buffer is empty");
        T value = data[head];
        head = (head + 1) % cap;
        count--;
        return value;
    }

    int size() const { return count; }
    bool empty() const { return count == 0; }
    bool full() const { return count == cap; }
    int capacity() const { return cap; }

    T& front()
    {
        if (empty()) throw std::runtime_error("Buffer is empty");
        return data[head];
    }

    const T& front() const
    {
        if (empty()) throw std::runtime_error("Buffer is empty");
        return data[head];
    }

    T& operator[](int i)
    {
        if (i < 0 || i >= count) throw std::out_of_range("Index out of range");
        return data[(head + i) % cap];
    }

    const T& operator[](int i) const
    {
        if (i < 0 || i >= count) throw std::out_of_range("Index out of range");
        return data[(head + i) % cap];
    }

private:
    int cap;
    std::unique_ptr<T[]> data;
    int head;
    int tail;
    int count;
};

int main()
{
    try
    {
        CircularBuffer<int> buffer(4);
        buffer.push(1);
        buffer.push(2);
        buffer.push(3);
        buffer.push(4);
        buffer.push(5);
        buffer.push(6);
        buffer.push(7);
        buffer.push(8);
        buffer.push(9);

        std::cout << "Buffer size: " << buffer.size() << std::endl;
        std::cout << "Buffer contents: ";
        for (int i = 0; i < buffer.size(); ++i)
        {
            std::cout << buffer[i] << " ";
        }
        std::cout << std::endl;

        while (!buffer.empty())
        {
            std::cout << "Popped: " << buffer.pop() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }
}