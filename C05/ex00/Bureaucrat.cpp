#include "Bureaucrat.hpp"

Bureaucrat::Bureaucrat(const std::string name, int grade) : name(name), grade(grade)
{
    if (grade < 1)
        throw(GradeTooHighException());
    else if (grade > 150)
        throw (GradeTooLowException());
}

Bureaucrat::Bureaucrat(const Bureaucrat &other) : name(other.name)
{
    grade = other.grade;
}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat &other)
{
    grade = other.grade;
    return (*this);
}

std::ostream& operator<<(std::ostream &os, Bureaucrat &Bureaucrat)
{
    os << Bureaucrat.getName() << ", bureaucrat grade " << Bureaucrat.getGrade();
    return (os);
}

std::string Bureaucrat::getName() const
{
    return (name);
}

int Bureaucrat::getGrade() const
{
    return(grade);
}

void Bureaucrat::increment()
{
    if (grade - 1 < 1)
        throw (GradeTooHighException());
    grade--;
}

void Bureaucrat::decrement()
{
    if (grade + 1 > 150)
        throw (GradeTooLowException());
    grade++;
}

Bureaucrat::~Bureaucrat()
{
}