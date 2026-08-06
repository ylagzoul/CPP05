#include <iostream>
#include <sstream>


int main(int ac, char *av[])
{
    (void)ac;
    std::stringstream str;
    str << av[1];

    float num;

    str >> num;

    if (str.fail())
        std::cout << "good test\n";
    else
        std::cout << "invaled test\n";

    std::cout << "num == " << num << std::endl;
}