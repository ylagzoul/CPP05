#include <iostream>

void test()
{
    throw 10;
}

int main()
{
    try
    {
        test();
    }
    catch(int x)
    {
        std::cerr << x << '\n';
    }
    
}