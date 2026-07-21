#include "Bureaucrat.hpp"

void ShrubberyCreationForm::beSigned(const Bureaucrat& b)
{
    if (b.getGrade() > 145)
        throw GradeTooLowException();
    _shrubbery = true;
}

void ShrubberyCreationForm::beExcec(const Bureaucrat& b)
{
    if (b.getGrade() > 137)
        throw GradeTooLowException();
    // creat file and cpy tree
}



