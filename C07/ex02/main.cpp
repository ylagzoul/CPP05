#include "Array.hpp"



int main()
{
    Array<int> a(10);
    a._data[3] = 132;
    for (size_t i = 0; i < a._size ;i++)
    {
        std::cout << a._data[i] << std::endl;
    }
}