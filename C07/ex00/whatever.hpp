#ifndef WHATEVER_HPP
# define WHATEVER_HPP

#include <iostream>
template <typename type>

type max(type a, type b)
{
    if (b >= a)
        return (b);
    else
        return (a);
}

template <typename type>

type min(type a, type b)
{
    if (b <= a)
        return (b);
    else
        return (a);
}

template <typename type>

void swap(type &a, type &b)
{
    type temp = a;
    a = b;
    b = temp;
}

#endif