#include "Bureaucrat.hpp"


int main()
{
    try
    {
        Bureaucrat bob("youssef", 20);
        std::cout << bob << std::endl;
        bob.increment();
        std::cout << bob << std::endl;
        bob.decrement();
        std::cout << bob << std::endl;
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "\n -------------------------- \n";
    try
    {
        Bureaucrat bob("youssef", 20);
        std::cout << bob << std::endl;
        bob.increment();
        std::cout << bob << std::endl;
        bob.decrement();
        std::cout << bob << std::endl;
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "\n -------------------------- \n";
    try
    {
        Bureaucrat bob2("hamza", -1);
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "\n -------------------------- \n";
    try
    {
        Bureaucrat bob2("hamza", 200);
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "\n -------------------------- \n";
    try
    {
        Bureaucrat bob2("hamza", 200);
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}