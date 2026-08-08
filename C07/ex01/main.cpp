#include "iter.hpp"

int main()
{
    int array[] = {1,23,57,100};
    size_t n = 4;
    iter(array, n, increment);
    iter(array, n, print);
    std::cout << "\n ------------------------------ \n";
    const int array2[] = {1,23,57,100};
    size_t n2 = 4;
    iter(array2, n2, print);
}