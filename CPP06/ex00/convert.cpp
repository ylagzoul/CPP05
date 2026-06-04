#include "convert.hpp"
// 42.6
// 42.6f

// a

// /////////
// nan
// nanf
// -inf
// -inff
// +inf
// +inff
// ///////
// char
// int
// float
// double
// type != "*"

ScalarConverter::ScalarConverter()
{
    Type_v = "notType";
}

int ScalarConverter::checkParameter(int ac, std::string par)
{
    int comma = 0;

    if (ac != 2)
        return (1);
    else if (par == "nan" || par == "nanf" || par == "-inf"
            || par == "-inff" || par == "+inf" || par == "+inff")
    {
        Type_v = "pseudo-literals";
    }
    else if (par.size() == 1 && !isdigit(par[0]))
        Type_v = "char";
    else
    {
        for(size_t i = 0; i < par.size()-1; i++)
        {
            if(!isdigit(par[i]))
            {
                if (par[i] == '.' && comma == 0)
                    comma++;
                else if (par[i] == 'f' &&  par[i] == '\0')
                    Type_v = "float";
                else
                    return (1);
            }
        }
        if (comma == 1 && Type_v == "notType")
            Type_v = "double";
        else if (comma == 0)
            Type_v = "int";
    }
    return (0);
}

void Convert_pseudoLiterals(std::string par)
{
    std::cout << "char: impossible" << std::endl;
    std::cout << "int: impossible" << std::endl;
    if (par == "nan" || par == "nanf")
    {
        std::cout << "float: nanf" << std::endl;
        std::cout << "double: nan" << std::endl;
    }
    else if (par == "-inf" || par == "-inff")
    {
        std::cout << "float: -inff" << std::endl;
        std::cout << "double: -inf" << std::endl;
    }
    else if (par == "+inf" || par == "+inff")
    {
        std::cout << "float: +inff" << std::endl;
        std::cout << "double: +inf" << std::endl;
    }
}


int main(int ac, char*av[])
{
    ScalarConverter convert;
    if (convert.checkParameter(ac,av[1]))
    {
        std::cout << "not valid input";
        return (1);
    }
    if (convert.Type_v == "pseudo-literals")
    {
        Convert_pseudoLiterals(av[1]);
    }

}