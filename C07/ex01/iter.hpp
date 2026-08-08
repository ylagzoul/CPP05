#ifndef ITER_HPP
#define ITER_HPP

#include <iostream>
template <typename T>

void print(const T& x)
{
    std::cout << x << std::endl;
}

template <typename T>
void increment(T& x)
{
    x++;
}

template <typename T>

void iter(T *array, const size_t size, void (*f)(T &x))
{
    for (size_t i = 0; i < size; i++)
    {
        f(array[i]);
    }
}

template<typename T>
void iter(const T *array, size_t len, void (*f)(const T &))
{
    for (size_t i = 0; i < len; i++)
        f(array[i]);
}

#endif