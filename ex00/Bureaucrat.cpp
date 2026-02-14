#include "Bureaucrat.hpp"

Bureaucrat::Bureaucrat(const std::string name, int grade) : name(name) , grade(grade)
{
    if (grade < 1)
        throw GradeTooHighException();
    else if (grade > 150)
        throw GradeTooLowException();
}

const std::string Bureaucrat::getName()
{
    return (name);
}

void Bureaucrat::getGrade()
{
    std::cout << grade;
}

void Bureaucrat::increment()
{
    if (grade - 1 < 1)
        throw GradeTooHighException();
    grade--;
}

void Bureaucrat::decrement()
{
    if (grade + 1 > 150)
        throw GradeTooLowException();
    grade++;
}

Bureaucrat::~Bureaucrat()
{
    ;
}