#include "Bureaucrat.hpp"

void RobotomyRequestForm::beSigned(const Bureaucrat& b)
{
    if (b.getGrade() > 72)
        throw GradeTooLowException();
    // _signed = true;
}

void RobotomyRequestForm::beExcec(const Bureaucrat& b)
{
    if (b.getGrade() > 45)
        throw GradeTooLowException();
    // _signed = true;
}