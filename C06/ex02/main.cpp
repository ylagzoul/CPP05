#include "Base.hpp"
#include <iostream>

Base* generate(void);
void identify(Base* p);
void identify(Base& p);

int main()
{
    Base* p = generate();

    std::cout << "Pointer version:" << std::endl;
    identify(p);

    std::cout << "\nReference version:" << std::endl;
    identify(*p);

    delete p;

    return 0;
}