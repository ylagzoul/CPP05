#include "ScalarConverter.hpp"



int main(int ac, char *av[])
{
    if (ac != 2)
    {
        std::cout << "invaled parameter\n";
    }
    else
    {
        std::cout << std::fixed << std::setprecision(1);
        ScalarConverter::convert(av[1]);
    }
}