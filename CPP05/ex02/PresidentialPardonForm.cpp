#include "Bureaucrat.hpp"


void PresidentialPardonForm::beSigned(const Bureaucrat& b)
{
    if (b.getGrade() > 25)
        throw GradeTooLowException();
    // _signed = true;
}

void PresidentialPardonForm::beExcec(const Bureaucrat& b)
{
    if (b.getGrade() > 5)
        throw GradeTooLowException();
    // _signed = true;
}