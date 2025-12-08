#include <iostream>
#include "unique_ptr.cpp"
#include "shared_weak_ptr.cpp"
#include <memory>

int main()
{
    int* raw_ptr = new int(10); //объект int(10) в куче
    UniquePtr<int> u_ptr(raw_ptr);
    //UniquePtr<int> copy_u_ptr(u_ptr);
    //UniquePtr<int> copy_u_ptr = u_ptr;
    UniquePtr<int> move_u_ptr(std::move(u_ptr));
    UniquePtr<int> move_u_ptr2 = std::move(u_ptr); //copy elision
    //UniquePtr<int> u_ptr2(u_ptr); уже второй владелец
    //обьекта int(10), так нельзя
    UniquePtr<int> move_u_ptr_with_operator_move;
    move_u_ptr_with_operator_move = std::move(move_u_ptr);
    std::cout << *move_u_ptr_with_operator_move;
    if (move_u_ptr == nullptr)
    {
        std::cout << "\nmove_u_ptr is nullptr";
    }
    std::cout << "\n\n\n\n";

    int* raw_memory = reinterpret_cast<int*>(malloc(4));
    SharedPtr<int> sh_ptr(raw_memory);
    std::cout << sh_ptr.use_count() << std::endl;

    SharedPtr<int> copy_of_sh_ptr(sh_ptr);
    std::cout << sh_ptr.use_count() << copy_of_sh_ptr.use_count() <<std::endl;

    SharedPtr<int> move_ptr(std::move(copy_of_sh_ptr));
    std::cout << move_ptr.use_count();

    SharedPtr<int> ptr1;
    ptr1 = move_ptr;
    std::cout << ptr1.use_count();


    int* rawMemory = new int(10);
    SharedPtr<int> sharedPtr(rawMemory);
    WeakPtr<int> weakPtr(sharedPtr);

    WeakPtr<int> weakPtr1;
    weakPtr1 = sharedPtr;

    SharedPtr<int> sharedPtr_controlling_weakPtr = weakPtr.lock();

    std::cout << sharedPtr.use_count();
}