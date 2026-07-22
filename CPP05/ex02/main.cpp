#include <iostream>

#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

int main()
{
    std::cout << "========== TEST 1 ==========" << std::endl;
    std::cout << "Shrubbery Creation" << std::endl;

    try
    {
        Bureaucrat bob("Bob", 100);
        ShrubberyCreationForm shrubbery("home");

        std::cout << "{1}" << bob << std::endl;
        std::cout << "{2}" << shrubbery << std::endl;

        bob.signForm(shrubbery);
        bob.executeForm(shrubbery);
    }
    catch (const std::exception& e)
    {
        std::cout << "{3}" << "Exception: "
                  << e.what()
                  << std::endl;
    }


    std::cout << "\n========== TEST 2 ==========" << std::endl;
    std::cout << "Robotomy Request" << std::endl;

    try
    {
        Bureaucrat robot("Robot", 40);
        RobotomyRequestForm robotomy("Bender");

        robot.signForm(robotomy);
        robot.executeForm(robotomy);
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception: "
                  << e.what()
                  << std::endl;
    }


    std::cout << "\n========== TEST 3 ==========" << std::endl;
    std::cout << "Presidential Pardon" << std::endl;

    try
    {
        Bureaucrat president("President", 1);
        PresidentialPardonForm pardon("Arthur");

        president.signForm(pardon);
        president.executeForm(pardon);
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception: "
                  << e.what()
                  << std::endl;
    }


    std::cout << "\n========== TEST 4 ==========" << std::endl;
    std::cout << "Execute unsigned form" << std::endl;

    try
    {
        Bureaucrat bob("Bob", 1);
        ShrubberyCreationForm shrubbery("garden");

        bob.executeForm(shrubbery);
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception: "
                  << e.what()
                  << std::endl;
    }


    std::cout << "\n========== TEST 5 ==========" << std::endl;
    std::cout << "Cannot sign form" << std::endl;

    try
    {
        Bureaucrat low("Low", 150);
        PresidentialPardonForm pardon("Arthur");

        low.signForm(pardon);
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception: "
                  << e.what()
                  << std::endl;
    }


    std::cout << "\n========== TEST 6 ==========" << std::endl;
    std::cout << "Can sign but cannot execute" << std::endl;

    try
    {
        Bureaucrat bureaucrat("John", 20);
        PresidentialPardonForm pardon("Arthur");

        bureaucrat.signForm(pardon);
        bureaucrat.executeForm(pardon);
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception: "
                  << e.what()
                  << std::endl;
    }


    std::cout << "\n========== TEST 7 ==========" << std::endl;
    std::cout << "Grade exceptions" << std::endl;

    try
    {
        Bureaucrat bad("Bad", 151);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what()
                  << std::endl;
    }

    try
    {
        Bureaucrat bad("Bad", 0);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what()
                  << std::endl;
    }

    return (0);
}