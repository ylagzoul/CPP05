#include "Bureaucrat.hpp"

void ShrubberyCreationForm::beSigned(const Bureaucrat& b)
{
    if (b.getGrade() > 145)
        throw GradeTooLowException();
    // _signed = true;
}