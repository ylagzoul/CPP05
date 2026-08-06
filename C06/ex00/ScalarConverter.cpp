#include "ScalarConverter.hpp"

ScalarConverter::ScalarConverter() {};

ScalarConverter::ScalarConverter(const ScalarConverter&) {};

ScalarConverter& ScalarConverter::operator=(const ScalarConverter&)
{
    return (*this);
}


void printImpossible()
{
    std::cout << "char: impossible" << std::endl;
    std::cout << "int: impossible" << std::endl;
    std::cout << "float: impossible" << std::endl;
    std::cout << "double: impossible" << std::endl;
}

void  pseudoLiterals(std::string &num)
{
    std::cout << "char: impossible" << std::endl;
    std::cout << "int: impossible" << std::endl;
    if (num == "nan" || num == "+inf" || num == "-inf")
    {
        std::cout << "float: " << num << "f" << std::endl;
        std::cout << "double: " << num << std::endl;
    }
    else
    {
        std::cout << "float: " << num << std::endl;
        num.erase(num.begin() +(num.size()-1));
        std::cout << "double: " << num << std::endl;
    }
}

void printChar(std::string& num)
{
    char c = num[0];

    std::cout << "char: '" << c << "'" << std::endl;
    std::cout << "int: " << static_cast<int>(c) << std::endl;
    std::cout << "float: " << static_cast<float>(c) << "f" << std::endl;
    std::cout << "double: " << static_cast<double>(c) << std::endl;
}

void printInt(std::stringstream &str)
{
    int nb;
    str >> nb;

    if (str.fail())
        printImpossible();
    else
    {
        if (nb < CHAR_MIN || nb > CHAR_MAX)
            std::cout << "char: impossible" << std::endl;
        else if (isprint(static_cast<char>(nb)))
            std::cout << "char: '" << static_cast<char>(nb) << "'" << std::endl;
        else
            std::cout << "char: Non displayable" << std::endl;
        
        std::cout << "int: " << nb << std::endl;
        std::cout << "float: " << static_cast<float>(nb) << "f" << std::endl;
        std::cout << "double: " << static_cast<double>(nb) << std::endl;
    }
}

void printFloat(std::stringstream &str)
{
    float nb;
    str >> nb;

    if (str.fail())
        printImpossible();
    else
    {
        if (nb < CHAR_MIN || nb > CHAR_MAX)
            std::cout << "char: impossible" << std::endl;
        else if (isprint(static_cast<char>(nb)))
            std::cout << "char: '" << static_cast<char>(nb) << "'" << std::endl;
        else
            std::cout << "char: Non displayable" << std::endl;

        if (nb < INT_MIN || nb > INT_MAX)
            std::cout << "int: impossible" << std::endl;
        else
            std::cout << "int: " << static_cast<int>(nb) << std::endl;

        std::cout << "float: " << nb << "f" << std::endl;
        std::cout << "double: " << static_cast<double>(nb) << std::endl;
    }
}

void printDouble(std::stringstream &str)
{
    double nb;
    str >> nb;

    if (str.fail())
        printImpossible();
    else
    {
        if (nb < CHAR_MIN || nb > CHAR_MAX)
            std::cout << "char: impfossible" << std::endl;
        else if (isprint(static_cast<char>(nb)))
            std::cout << "char: '" << static_cast<char>(nb) << "'" << std::endl;
        else
            std::cout << "char: Non displayable" << std::endl;

        if (nb < INT_MIN || nb > INT_MAX)
            std::cout << "int: impossible" << std::endl;
        else
            std::cout << "int: " << static_cast<int>(nb) << std::endl;

        if (nb < std::numeric_limits<float>::min() || nb > std::numeric_limits<float>::max())
            std::cout << "float: impossible" << std::endl;
        else
            std::cout << "float: " << static_cast<float>(nb) << "f" << std::endl;
        std::cout << "double: " << nb << std::endl;
    }
}


void printType(std::string Type, std::string& num)
{
    if (Type == "float")
        num.erase(num.begin() + num.size()-1);
    std::stringstream str;
    str << num;
    if (Type == "char")
        printChar(num);
    else if (Type == "int")
        printInt(str);
    else if (Type == "float")
        printFloat(str);
    else if (Type == "double")
        printDouble(str);
    else if (Type == "impossible")
        printImpossible(); 
}


void ScalarConverter::convert(std::string num)
{
    std::string Type = "impossible";
    std::cout << "num.size() = " << num.size()<< "\n";
    if (num == "nan" || num == "+inf" || num == "-inf" || num == "nanf" || num == "+inff" || num == "-inff")
        pseudoLiterals(num);
    else
    {
        if (num.size() == 1 && !isdigit(num[0]))
            Type = "char";
        else
        {
            int cont = 0;
            for (size_t i = 0; i < num.size(); i++)
            {
                if (!isdigit(num[i]))
                {
                    if (num[i] == '-' && i == 0)
                        continue ;
                    if (num[i] == '.' && cont == 0)
                    {
                        Type = "double";
                        cont++;
                    }
                    else if (num[i] == 'f' && cont == 1)
                    {
                        if (i != num.size()-1)
                        {
                            Type = "impossible";
                            break ;
                        }
                        Type = "float";
                    }
                    else
                    {
                        Type = "impossible";
                        break ;
                    }
                }
                if (Type == "impossible")
                    Type = "int";
            }
        }
        printType(Type, num);
    }
}



ScalarConverter::~ScalarConverter() {};
